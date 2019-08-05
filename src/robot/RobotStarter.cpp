#include "RobotStarter.h"
#include "Robot.h"
#include "Conveyor.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include <fstream>
#include <vector>
#include <functional>
#include "ff.h"

#include "smoothie/Module.h"
#include "smoothie/GCode.h"
#include "smoothie/GCodeProcessor.h"
#include "smoothie/Dispatcher.h"

#include "libs/OutputStream.h"
#include "libs/MessageQueue.h"
#include "libs/RingBuffer.h"

#include "_hal/__hal.h"
#include "_hal/board.h"
#include "_hal/uart.h"
#include "_hal/Pin/AdcPin.h"
#include "_hal/Pin/PwmPin.h"
#include "_hal/Pin/OutputPin.h"
#include "_hal/stopwatch.h"
#include "_hal/spiffs_ext.h"
#include "component/i2c.h"
#include "component/ads1115.h"
// static const char *TAG = "StartUp";

static bool system_running= false;
static bool rpi_port_enabled= false;
static uint32_t rpi_baudrate= 115200;
static Pin *aux_play_led = nullptr;

// for ?, $I or $S queries
// for ? then query_line will be nullptr
struct query_t
{
    OutputStream *query_os;
    char *query_line;
};
static RingBuffer<struct query_t, 8> queries; // thread safe FIFO

// set to true when M28 is in effect
static bool uploading = false;
static FILE *upload_fp = nullptr;
static std::string config_error_msg;

// TODO maybe move to Dispatcher
static GCodeProcessor gp;
static bool loaded_configuration= false;
static bool config_override= false;
const char *OVERRIDE_FILE= "/sd/config-override";

MemoryPool *_RAM2;
MemoryPool *_RAM3;
MemoryPool *_RAM4;
MemoryPool *_RAM5;

// Called very early from ResetISR()
void setup_memory_pool()
{
    // MemoryPool stuff - needs to be initialised before __libc_init_array
    // so static ctors can use them
    extern uint8_t __end_bss_RAM2;
    extern uint8_t __top_RAM2;
    extern uint8_t __end_bss_RAM3;
    extern uint8_t __top_RAM3;
    extern uint8_t __end_bss_RAM4;
    extern uint8_t __top_RAM4;
    extern uint8_t __end_bss_RAM5;
    extern uint8_t __top_RAM5;

    // alocate remaining areas to HEAP for those areas
    _RAM2= new MemoryPool(&__end_bss_RAM2, &__top_RAM2 - &__end_bss_RAM2);
    _RAM3= new MemoryPool(&__end_bss_RAM3, &__top_RAM3 - &__end_bss_RAM3);
    _RAM4= new MemoryPool(&__end_bss_RAM4, &__top_RAM4 - &__end_bss_RAM4);
    _RAM5= new MemoryPool(&__end_bss_RAM5, &__top_RAM5 - &__end_bss_RAM5);
}

// load configuration from override file
static bool load_config_override(OutputStream& os)
{
    std::fstream fsin(OVERRIDE_FILE, std::fstream::in);
    if(fsin.is_open()) {
        std::string s;
        OutputStream nullos;
        // foreach line dispatch it
        while (std::getline(fsin, s)) {
            if(s[0] == ';') continue;
            // Parse the Gcode
            GCodeProcessor::GCodes_t gcodes;
            gp.parse(s.c_str(), gcodes);
            // dispatch it
            for(auto& i : gcodes) {
                if(i.get_code() >= 500 && i.get_code() <= 503) continue; // avoid recursion death
                if(!THEDISPATCHER->dispatch(i, nullos)) {
                    os.printf("[W][Startup]WARNING: load_config_override: this line was not handled: %s\n", s.c_str());
                }
            }
        }
        loaded_configuration= true;
        fsin.close();

    }else{
        loaded_configuration= false;
        return false;
    }

    return true;
}

// can be called by modules when in command thread context
bool dispatch_line(OutputStream& os, const char *cl)
{
    // printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
    // Don't like this, but we need a writable copy of the input line
    char line[strlen(cl) + 1];
    strcpy(line, cl);

    // map some special M codes to commands as they violate the gcode spec and pass a string parameter
    // M23, M32, M117, M30 => m23, m32, m117, rm and handle as a command
    if(strncmp(line, "M23 ", 4) == 0) line[0] = 'm';
    else if(strncmp(line, "M30 ", 4) == 0) { strcpy(line, "rm /sd/"); strcpy(&line[7], &cl[4]); } // make into an rm command
    else if(strncmp(line, "M32 ", 4) == 0) line[0] = 'm';
    else if(strncmp(line, "M117 ", 5) == 0) line[0] = 'm';

    // handle save to file M codes:- M28 filename, and M29
    if(strncmp(line, "M28 ", 4) == 0) {
        char *upload_filename = &line[4];
        if(strncmp(upload_filename, "/sd/", 4) != 0) {
            // prepend /sd/ luckily we have exactly 4 characters before the filename
            memcpy(line, "/sd/", 4);
            upload_filename = line;
        }
        upload_fp = fopen(upload_filename, "w");
        if(upload_fp != nullptr) {
            uploading = true;
            os.printf("Writing to file: %s\nok\n", upload_filename);
        } else {
            os.printf("open failed, File: %s.\nok\n", upload_filename);
        }
        return true;
    }

    // see if a command
    if(islower(line[0]) || line[0] == '$') {

        // we could handle this in CommandShell
        if(line[0] == '$' && strlen(line) >= 2) {
            if(line[1] == 'X') {
                // handle $X
                if(Module::is_halted()) {
                    Module::broadcast_halt(false);
                    os.puts("[Caution: Unlocked]\nok\n");
                } else {
                    os.puts("ok\n");
                }
                return true;
            }
        }

        // dispatch command
        if(!THEDISPATCHER->dispatch(line, os)) {
            if(line[0] == '$') {
                os.puts("error:Invalid statement\n");
            } else {
                os.printf("error:Unsupported command - %s\n", line);
            }

        }else if(!os.is_no_response()) {
            os.puts("ok\n");
        }else{
            os.set_no_response(false);
        }

        return true;
    }

    //printf("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq\n");
    // Handle Gcode
    GCodeProcessor::GCodes_t gcodes;

    // Parse gcode
    if(!gp.parse(line, gcodes)) {
        // line failed checksum, send resend request
        os.printf("rs N%d\n", gp.get_line_number() + 1);
        return true;

    } else if(gcodes.empty()) {
        // if gcodes is empty then was a M110, just send ok
        os.puts("ok\n");
        return true;
    }
    //printf("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n");

    // if in M28 mode then just save all incoming lines to the file until we get M29
    if(uploading && gcodes[0].has_m() && gcodes[0].get_code() == 29) {
        // done uploading, close file
        fclose(upload_fp);
        upload_fp = nullptr;
        uploading = false;
        os.printf("Done saving file.\nok\n");
        return true;
    }
    //printf("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n");

    // dispatch gcodes
    // NOTE return one ok per line instead of per GCode only works for regular gcodes like G0-G3, G92 etc
    // gcodes returning data like M114 should NOT be put on multi gcode lines.
    int ngcodes = gcodes.size();
    for(auto& i : gcodes) {
        i.dump(os);     //
        if(i.has_m() || i.has_g()) {
            // printf("------------ AAA\n");   
            if(uploading) {
                // just save the gcodes to the file
                if(upload_fp != nullptr) {
                    // write out gcode
                    i.dump(upload_fp);
                }

                os.printf("ok\n");
                return true;
            }
            //printf("------------bbb\n");
            // potentially handle M500 - M503 here
            OutputStream *pos= &os;
            std::fstream *fsout= nullptr;
            bool m500= false;

            if(i.has_m() && (i.get_code() >= 500 && i.get_code() <= 503)) {
                if(i.get_code() == 500) {
                    // we have M500 so redirect os to a config-override file
                    fsout= new std::fstream(OVERRIDE_FILE, std::fstream::out | std::fstream::trunc);
                    if(!fsout->is_open()) {
                        os.printf("ERROR: opening file: %s\n", OVERRIDE_FILE);
                        delete fsout;
                        return true;
                    }
                    pos= new OutputStream(fsout);
                    m500= true;

                } else if(i.get_code() == 501) {
                    if(load_config_override(os)) {
                        os.printf("configuration override loaded\nok\n");
                    }else{
                        os.printf("failed to load configuration override\nok\n");
                    }
                    return true;

                } else if(i.get_code() == 502) {
                    remove(OVERRIDE_FILE);
                    os.printf("configuration override file deleted\nok\n");
                    return true;

                } else if(i.get_code() == 503) {
                    if(loaded_configuration) {
                        os.printf("// NOTE: config override loaded\n");
                    }else{
                        os.printf("// NOTE: No config override loaded\n");
                    }
                    i.set_command('M', 500, 3); // change gcode to be M500.3
                }
            }
            // printf("------------ccc\n");

            // if this is a multi gcode line then dispatch must not send ok unless this is the last one
            if(!THEDISPATCHER->dispatch(i, *pos, ngcodes == 1 && !m500)) {
                // no handler processed this gcode, return ok - ignored
                if(ngcodes == 1) os.puts("ok - ignored\n");
            }
            // printf("------------ddd\n");

            // clean up after M500
            if(m500) {
                printf("------------fffffffff\n");

                m500= false;
                fsout->close();
                delete fsout;
                delete pos; // this would be the file output stream
                if(!config_override) {
                    os.printf("WARNING: override will NOT be loaded on boot\n", OVERRIDE_FILE);
                }
                os.printf("Settings Stored to %s\nok\n", OVERRIDE_FILE);
            }
            // printf("------------ggg\n");

        } else {
            // if it has neither g or m then it was a blank line or comment
            os.puts("ok\n");
        }
        // printf("------------hhhhhhhh\n");
        --ngcodes;
    }
    // printf("------------kkkkkkkk\n");

    return true;
}

static std::function<void(char)> capture_fnc;
void set_capture(std::function<void(char)> cf)
{
    capture_fnc = cf;
}

static std::vector<OutputStream*> output_streams;

#include "stdio.h"
#include "_hal/patch.h"
char debug_const[100];
// this is here so we do not need to duplicate this logic for USB and UART
void process_command_buffer(size_t n, char *rx_buf, OutputStream *os, char *line, size_t& cnt, bool& discard)
{
    // os->puts("[DEBUG] Start process_command_buffer()\n");
    for (size_t i = 0; i < n; ++i) {
        line[cnt] = rx_buf[i];
        // os->puts(&rx_buf[i]);
        // os->puts(line);
        // os->puts("\n");
        if(capture_fnc) {      // What does it mean?  Jun 2019
            // os->puts("capture_fuc == true \n");
            capture_fnc(line[cnt]);
            continue;
        }

        if(line[cnt] == 24) { // ^X
            if(!Module::is_halted()) {
                Module::broadcast_halt(true);
                os->puts("ALARM: Abort during cycle\n");
            }
            discard = false;
            cnt = 0;

        } else if(line[cnt] == '?') {
            if(!queries.full()) {
                os->puts("--- query \n");
                queries.push_back({os, nullptr});
            }

        } else if(discard) {
            // we discard long lines until we get the newline
            if(line[cnt] == '\n') discard = false;

        } else if(cnt >= MAX_LINE_LENGTH - 1) {
            // discard long lines
            discard = true;
            cnt = 0;
            os->puts("error:Discarding long line\n");

        } else if(line[cnt] == char(10)) {    // "\n" == char(10) == LF   char(80)='P'   char(13)=CR
            // os->puts("---- end of line\n");
            os->clear_flags(); // clear the done flag here to avoid race conditions
            line[cnt] = '\0'; // remove the \n and nul terminate
            // os->puts(line);
            // os->puts("\n");
            //     debug_const[0] = cnt & 0xff;
            // debug_const[1] = (cnt >> 8)  & 0xff;
            // debug_const[2] = (cnt >> 16) & 0xff;
            // debug_const[3] = (cnt >> 24) & 0xff;
            // debug_const[2]=0;
            // os->puts(debug_const);

            if(cnt == 2 && line[0] == '$' && (line[1] == 'I' || line[1] == 'S')) {
                // Handle $I and $S as instant queries
                if(!queries.full()) {
                    queries.push_back({os, strdup(line)});
                }

            }else{
                // os->puts("sending message queue. \n");
                send_message_queue(line, os);
            }
            cnt = 0;

        } else if(line[cnt] == '\r') {
            // ignore CR
            continue;

        } else if(line[cnt] == 8 || line[cnt] == 127) { // BS or DEL
            if(cnt > 0) --cnt;

        } else {
            ++cnt;
        }
    }
}

#include "Arduino.h"
static void uart_comms(void *)
{
    printf("DEBUG: UART Comms thread running\n");
    set_notification_uart(xTaskGetCurrentTaskHandle());

    // create an output stream that writes to the uart
    static OutputStream os([](const char *buf, size_t len) { return write_uart(buf, len); });
    output_streams.push_back(&os);

    const TickType_t waitms = pdMS_TO_TICKS( 300 );

    char rx_buf[256];
    char line[MAX_LINE_LENGTH];
    size_t cnt = 0;
    bool discard = false;
    // for(char x: rx_buf){x = char(0);}
    for(int i=0; i< 256;i++){rx_buf[i] = 0;}
    // for(char x: line){x = char(0);}
    for(int i=0; i< MAX_LINE_LENGTH;i++){line[i] = 0;}

    while(1) {
        // Wait to be notified that there has been a UART irq. (it may have been rx or tx so may not be anything to read)
        uint32_t ulNotificationValue = ulTaskNotifyTake( pdTRUE, waitms );

        if( ulNotificationValue != 1 ) {
            /* The call to ulTaskNotifyTake() timed out. check anyway */   
            //It is always == 0;
            // Serial.print("ulNotificationValue = ");
            // Serial.println(ulNotificationValue);
        }

        size_t n = read_uart(rx_buf, sizeof(rx_buf));
        if(n > 0) {
            //os.puts("[I][task.uart] got rs buffer not empty...\n");
            // Serial.print("===============before. Line cnt = ");
            // Serial.println(cnt);
            // Serial.println(line);
            // Serial.print("----------------before. rxbuffer n = ");
            // Serial.println(n);
            // Serial.println(rx_buf);

            process_command_buffer(n, rx_buf, &os, line, cnt, discard);
            // Serial.print("====================after. line cnt = ");
            // Serial.println(cnt);
            // Serial.println(line);
            // Serial.print("-----------------------after. rx n = ");
            // Serial.println(n);
            // Serial.println(rx_buf);
        }
    }
}
#ifdef BOARD_PRIMEALPHA
static void uart3_comms(void *)
{
    printf("DEBUG: UART3 Comms thread running\n");
    set_notification_uart3(xTaskGetCurrentTaskHandle());

    // create an output stream that writes to the uart
    static OutputStream os([](const char *buf, size_t len) { return write_uart3(buf, len); });
    output_streams.push_back(&os);

    const TickType_t waitms = pdMS_TO_TICKS( 300 );

    char rx_buf[256];
    char line[MAX_LINE_LENGTH];
    size_t cnt = 0;
    bool discard = false;
    while(1) {
        // Wait to be notified that there has been a UART irq. (it may have been rx or tx so may not be anything to read)
        uint32_t ulNotificationValue = ulTaskNotifyTake( pdTRUE, waitms );

        if( ulNotificationValue != 1 ) {
            /* The call to ulTaskNotifyTake() timed out. check anyway */
        }

        size_t n = read_uart3(rx_buf, sizeof(rx_buf));
        if(n > 0) {
           process_command_buffer(n, rx_buf, &os, line, cnt, discard);
        }
    }
}
#endif

// this prints the string to all consoles that are connected and active
// must be called in command thread context
void print_to_all_consoles(const char *str)
{
    for(auto i : output_streams) {
        i->puts(str);
    }
}

static void handle_query(bool need_done)
{
    // set in comms thread, and executed in the command thread to avoid thread clashes.
    // the trouble with this is that ? does not reply if a long command is blocking call to dispatch_line
    // test commands for instance or a long line when the queue is full or G4 etc
    // so long as safe_sleep() is called then this will still be processed
    // also dispatch any instant queries we have recieved
    while(!queries.empty()) {
        struct query_t q= queries.pop_front();
        if(q.query_line == nullptr) { // it is a ? query
            std::string r;
            Robot::getInstance()->get_query_string(r);
            q.query_os->puts(r.c_str());

        } else {
            Dispatcher::getInstance()->dispatch(q.query_line, *q.query_os);
            free(q.query_line);
        }
        // on last one (Does presume they are the same os though)
        // FIXME may not work as expected when there are multiple I/O channels and output streams
        if(need_done && queries.empty()) q.query_os->set_done();
    }
}

/*
 * All commands must be executed in the context of this thread. It is equivalent to the main_loop in v1.
 * Commands are sent to this thread via the message queue from things that can block (like I/O)
 * Other things can call dispatch_line direct from the in_command_ctx call.
 */
static void command_handler()
{
    printf("DEBUG: Command thread running\n\n\n\n");
    int counter = 0;

    for(;;) {
        char *line;
        OutputStream *os= nullptr;
        bool idle = false;

        // This will timeout after 100 ms
        if(receive_message_queue(&line, &os)) {
            printf("DEBUG: got line: %s\n", line);
            dispatch_line(*os, line);
            handle_query(false);
            os->set_done(); // set after all possible output

        } else {
            // timed out or other error

            idle = true;
            if(config_error_msg.empty()) {
                // toggle led to show we are alive, but idle
                Board_LED_Toggle(0);
            }
            handle_query(true);
        }
        // call in_command_ctx for all modules that want it
        // dispatch_line can be called from that
        Module::broadcast_in_commmand_ctx(idle);

        // we check the queue to see if it is ready to run
        // we specifically deal with this in append_block, but need to check for other places
        if(Conveyor::getInstance() != nullptr) {
            Conveyor::getInstance()->check_queue();
        }

        counter++;
        if(counter % 100 == 0){
            //printf("command_handler run times = %i \n",counter);
        }
    }
}
// called only in command thread context, it will sleep (and yield) thread but will also
// process things like instant query
void safe_sleep(uint32_t ms)
{
    // here we need to sleep (and yield) for 10ms then check if we need to handle the query command
    TickType_t delayms = pdMS_TO_TICKS(10); // 10 ms sleep
    while(ms > 0) {
        vTaskDelay(delayms);
        // presumably there is a long running command that
        // may need Outputstream which will set done flag when it is done
        handle_query(false);

        if(ms > 10) {
            ms -= 10;
        } else {
            break;
        }
    }
}

#include "smoothie/CommandShell.h"
#include "libs/SlowTicker.h"
#include "libs/FastTicker.h"
#include "robot/StepTicker.h"
#include "smoothie/ConfigReader.h"
#include "modules/tools/switch/Switch.h"
#include "robot/Planner.h"
#include "robot/Robot.h"
#include "modules/utils/killbutton/KillButton.h"
#include "modules/tools/extruder/Extruder.h"
#include "modules/tools/temperaturecontrol/TemperatureControl.h"
#include "modules/utils/currentcontrol/CurrentControl.h"
#include "modules/tools/laser/Laser.h"
#include "modules/tools/endstops/Endstops.h"
#include "modules/tools/zprobe/ZProbe.h"
#include "modules/utils/player/Player.h"

// #include "Adc.h"
// #include "Pwm.h"


//float get_pll1_clk();

#define CONFIG_SOURCE_SD 1
#define CONFIG_SOURCE_SPIFFS 2
#define CONFIG_SOURCE_CODED_STRING 3
#define CONFIG_SOURCE 2

#if CONFIG_SOURCE == CONFIG_SOURCE_CODED_STRING
//Xuming>>>
#include STRING_CONFIG_H    //not impliment in origin SmoothieV2
static std::string str(string_config);
static std::stringstream ss(str);
//<<<Xuming
#endif

#if CONFIG_SOURCE == CONFIG_SOURCE_SD
extern "C" bool setup_sdmmc();
#endif

#if CONFIG_SOURCE == CONFIG_SOURCE_SPIFFS

#endif

// voltage monitors
static std::map<std::string, AdcPin*> voltage_monitors;

float get_voltage_monitor(const char* name)
{
    auto p= voltage_monitors.find(name);
    if(p == voltage_monitors.end()) return 0;
    return p->second->read_voltage();
}

int get_voltage_monitor_names(const char *names[])
{
    int i= 0;
    for(auto& p : voltage_monitors) {
        if(names != nullptr)
            names[i]= p.first.c_str();
        ++i;
    }
    return i;
}

// this is used to add callback functions to be called once the system is running
static std::vector<StartupFunc_t> startup_fncs;
void register_startup(StartupFunc_t sf)
{
    startup_fncs.push_back(sf);
}


//get general system settings
void setup_section_genenal(ConfigReader cr){
    ConfigReader::section_map_t m;
    if(cr.get_section("general", m)) {
        bool f = cr.get_bool(m, "grbl_mode", false);
        THEDISPATCHER->set_grbl_mode(f);
        printf("INFO: grbl mode %s\n", f ? "set" : "not set");
        config_override= cr.get_bool(m, "config-override", false);
        printf("INFO: use config override is %s\n", config_override ? "set" : "not set");
        rpi_port_enabled= cr.get_bool(m, "rpi_port_enable", false);
        rpi_baudrate= cr.get_int(m, "rpi_baudrate", 115200);
        printf("INFO: rpi port is %senabled, at baudrate: %lu\n", rpi_port_enabled ? "" : "not ", rpi_baudrate);
        std::string p = cr.get_string(m, "aux_play_led", "nc");
        aux_play_led = new OutputPin(p.c_str());
        if(!aux_play_led->connected()) {
            delete aux_play_led;
            aux_play_led = nullptr;
            printf("INFO: auxilliary play led is not avaliable.\n");
        }else{
            printf("INFO: auxilliary play led set to %s\n", aux_play_led->to_string().c_str());
        }
    }
}

// configure core modules here
void setup_section_core(ConfigReader cr){
    PwmPin::init_channel_flags();
    // PwmPin::xxx = 0;
    // Pwm needs to be initialized, there can only be one frequency
    // needs to be done before any module that could use it
    // uint32_t freq = 10000; // default is 10KHz
    // ConfigReader::section_map_t m;
    // if(cr.get_section("pwm", m)) {
    //     freq = cr.get_int(m, "frequency", freq);
    // }

    //Xuming Aug 2019
    //Esp32 pwm channels are working at different frequency. It will be handled by individual channel.
    // PwmPin::setup(freq);
    // printf("INFO: PWM frequency set to %d Hz\n", freq);
}

void setup_section_extruder(ConfigReader cr){
    printf("DEBUG: configure extruder\n");
    // this creates any configured extruders then we can remove it
    Extruder ex("extruder loader");
    if(!ex.configure(cr)) {
        printf("INFO: no Extruders loaded\n");
    }
}

esphome::ads1115::ADS1115Component* ads1115_component;
void setup_section_temperature_control(ConfigReader cr){
    if(AdcPin::setup()) {
        // this creates any configured temperature controls
        if(!TemperatureControl::load_controls(cr)) {
            printf("INFO: no Temperature Controls loaded\n");
        }
        // printf("[OK][setup.temperature.control] aaaaaaaaaaaaaaaaa \n");
    } else {
        printf("ERROR: ADC failed to setup\n");
    }
    //  printf ("[OK][setup.temperature.controls] bbbbbbbbbbbbbbbbbbb\n");
}

// configure the board: i2c, spi, s2c, etc...
void setup_section_bus(ConfigReader cr){
    #define scl_pin_key "scl_pin"
    #define sda_pin_key "sda_pin"
    
    ConfigReader::sub_section_map_t sub_section_bus;
    if(!cr.get_sub_sections("bus", sub_section_bus)) {
        printf("[E][RobotStart][Config][Bus] ERROR:configure-bus: no bus section found\n");
        return;
    }
    auto target_i2c = sub_section_bus.find("i2c_ads1115");
    if(target_i2c == sub_section_bus.end()) {
        printf("[E][RobotStart][setup_section_bus()] can't find i2c_ads1115.xxx\n");
        return; 
    }

    auto& this_i2c = target_i2c->second; // map of ic2 config values for this i2c
    OutputPin ads1115_scl_pin(cr.get_string(this_i2c, scl_pin_key, "nc"));
    InputPin adc1115_sda_pin(cr.get_string(this_i2c, sda_pin_key, "nc"));

    esphome::i2c::I2CComponent* i2c_component =new esphome::i2c::I2CComponent();
    i2c_component->set_scl_pin(ads1115_scl_pin.get_gpio_id());
    i2c_component->set_sda_pin(adc1115_sda_pin.get_gpio_id());
    i2c_component->set_frequency(400000);
    i2c_component->set_scan(false);
    i2c_component->dump_config();   //Doesn't work!
    i2c_component->setup();
    printf("-----I2CComponent\n");
    // ads1115_sensor.set_icon
    ads1115_component = new esphome::ads1115::ADS1115Component();
    ads1115_component->set_i2c_parent(i2c_component);
    ads1115_component->set_i2c_address(0x48);
    ads1115_component->setup();
    printf("-----ADS1115Component\n");

}

// configure voltage monitors if any
void setup_section_voltage_monitors(ConfigReader cr){
    ConfigReader::section_map_t m;
    if(cr.get_section("voltage monitor", m)) {
        for(auto& s : m) {
            std::string k = s.first;
            std::string v = s.second;

            AdcPin *padc= new AdcPin;
            if(padc->from_string(v.c_str()) == nullptr) {
                printf("WARNING: Failed to create %s voltage monitor\n", k.c_str());
                delete padc;
            }else{
                voltage_monitors[k]= padc;
                printf("DEBUG: added voltage monitor %s: %s\n", k.c_str(), v.c_str());
            }
        }
    }
    printf("[OK][setup] configure voltage monitors .........\n");
}

#include "Esp.h"   
void smoothie_startup(void *)
{
    printf("INFO: Smoothie V2.alpha Build for %s - starting up\n", BUILD_TARGET);
    //get_pll1_clk();

    // led 4 indicates boot phase 2 starts
    Board_LED_Set(3, true);
    // create the SlowTicker here as it is used by some modules
    SlowTicker *slow_ticker = new SlowTicker();

    // create the FastTicker here as it is used by some modules
    FastTicker *fast_ticker = new FastTicker();

    // create the StepTicker, don't start it yet
    StepTicker *step_ticker = new StepTicker();
#ifdef DEBUG
    // when debug is enabled we cannot run stepticker at full speed
    step_ticker->set_frequency(10000); // 10KHz
#else
    step_ticker->set_frequency(150000); // 150KHz
#endif
    step_ticker->set_unstep_time(1); // 1us step pulse by default

    // configure the Dispatcher
    new Dispatcher();

    bool ok = false;
    // open the config file
    do {
#if CONFIG_SOURCE == CONFIG_SOURCE_SD
        static FATFS fatfs; /* File system object */
        if(!setup_sdmmc()) {
            std::cout << "Error: setting up sdmmc\n";
            break;
        }
        // TODO check the card is inserted

        int ret = f_mount(&fatfs, "sd", 1);
        if(FR_OK != ret) {
            std::cout << "Error: mounting: " << "/sd: " << ret << "\n";
            break;
        }

        std::fstream fs;
        fs.open("/sd/config.ini", std::fstream::in);
        if(!fs.is_open()) {
            std::cout << "Error: opening file: " << "/sd/config.ini" << "\n";
            // unmount sdcard
            //f_unmount("sd");
            break;
        }


        ConfigReader cr(fs);
        printf("DEBUG: Starting configuration of modules from sdcard...\n");
#endif
#if CONFIG_SOURCE == CONFIG_SOURCE_CODED_STRING
        ConfigReader cr(ss);
        printf("DEBUG: Starting configuration of modules from memory...\n");
#endif
#if CONFIG_SOURCE == CONFIG_SOURCE_SPIFFS
        std::string std_string = spiffs_reading();
        std::stringstream std_string_stream(std_string);
        ConfigReader cr(std_string_stream);
#endif
        printf(" *************************** loading from config.ini *************************** \n");
        setup_section_bus(cr);

        Planner *planner = new Planner();
        Conveyor *conveyor = new Conveyor();
        Robot *robot = new Robot();
        setup_section_genenal(cr);
        planner->configure(cr);
        conveyor->configure(cr);
        if(!robot->configure(cr)) {
            printf("ERROR: Configuring robot failed\n");
            break;
        }
        setup_section_core(cr);
        setup_section_extruder(cr);
        setup_section_temperature_control(cr);

        // create all registered modules, the addresses are stored in a known location in flash
        //Xuming>>>
        // extern uint32_t __registered_modules_start;
        // extern uint32_t __registered_modules_end;
        // uint32_t *g_pfnModules= &__registered_modules_start;
        // printf("[DEBUG][registered_modules]aaaaaaaaaaaaaaa\n");
        // int i=0;
        // while (g_pfnModules < &__registered_modules_end) {
        //     printf("------ index = %i\n",i);
        //     i++;

        //     uint32_t *addr= g_pfnModules++;
        //     bool (*pfnModule)(ConfigReader& cr)= (bool (*)(ConfigReader& cr))*addr;
        //     // this calls the registered create function for the module
        //     pfnModule(cr);
        // }
        //printf("[OK][registered_modules].............\n");
        //<<<Xuming
        // end of module creation and configuration

        setup_section_voltage_monitors(cr);
        
#if CONFIG_SOURCE == CONFIG_SOURCE_SD
        // close the file stream
        fs.close();

        // unmount sdcard
        //f_unmount("sd");
#endif

        // initialize planner before conveyor this is when block queue is created
        // which needs to know how many actuators there are, which it gets from robot
        if(!planner->initialize(robot->get_number_registered_motors())) {
            printf("FATAL: planner failed to initialize, out of memory?\n");
            break;
        }

        // start conveyor last
        conveyor->start();

        printf("DEBUG: ...Ending configuration of modules\n");
        ok = true;
    } while(0);

    // create the commandshell, it is dependent on some of the above
    CommandShell *shell = new CommandShell();
    shell->initialize();

    if(ok) {
        printf("[DEBUG][setup.ok] Yes. \n");
        // start the timers
        if(!slow_ticker->start()) {
            printf("Error: failed to start SlowTicker\n");
        }

        if(!fast_ticker->start()) {
            printf("WARNING: failed to start FastTicker (maybe nothing is using it?)\n");
        }

        if(!step_ticker->start()) {
            printf("Error: failed to start StepTicker\n");
        }

        if(!AdcPin::start()) {
            printf("Error: failed to start ADC\n");
        }
        

    } else {
        printf("[DEBUG][setup.ok] nnnnnnnnnnnnnnnnnnnnn \n");

        puts("ERROR: Configure failed\n");
        config_error_msg= "There was a fatal error in the config.ini this must be fixed to continue\nOnly some shell commands are allowed and sdcard access\n";
        Module::broadcast_halt(true);
        puts(config_error_msg.c_str());
    }

    // create queue for incoming buffers from the I/O ports
    if(!create_message_queue()) {
        // Failed to create the queue.
        printf("Error: failed to create comms i/o queue\n");
    }
    printf("[OK][message_queue] is created.\n");
    // Start comms threads Higher priority than the command thread
    // fixed stack size of 4k Bytes each
    xTaskCreate(uart_comms, "UARTCommsThread", 1500, NULL, (tskIDLE_PRIORITY + 3UL), (TaskHandle_t *) NULL);
    printf("[OK][task.startup] created task.UARTCommsThread. \n");

    // run any startup functions that have been registered
    for(auto f : startup_fncs) {
        f();
    }
    startup_fncs.clear();
    startup_fncs.shrink_to_fit();

#ifdef BOARD_PRIMEALPHA
    if(rpi_port_enabled) {
        if(setup_uart3(rpi_baudrate) < 0) {
            printf("ERROR: UART3/RPI setup failed\n");
        } else {
            xTaskCreate(uart3_comms, "UART3CommsThread", 1500/4, NULL, (tskIDLE_PRIORITY + 3UL), (TaskHandle_t *) NULL
                );
        }
    }
#endif

    uint32_t free_heap = ESP.getFreeHeap(); 
    printf("------------------------------------------------------------------------------------------\n");  
    printf("DEBUG: Initial: free malloc memory= %d, free sbrk memory= %d, Total free= %d\n", free_heap, xPortGetFreeHeapSize() - free_heap, xPortGetFreeHeapSize());
    printf("[esp_get_free_heap_size()] free heap size = %d \n",esp_get_free_heap_size());   //Are they same? NO! WHY ?
    printf("------------------------------------------------------------------------------------------\n");  

    // indicate we are up and running
    system_running= true;

    // load config override if set
    if(ok && config_override) {
        OutputStream os(&std::cout);
        if(load_config_override(os)) {
            os.printf("INFO: configuration override loaded\n");

        }else{
            os.printf("INFO: No saved configuration override\n");
        }
    }

    // led 3,4 off indicates boot phase 2 complete
    Board_LED_Set(2, false);
    Board_LED_Set(3, false);

    // run the command handler in this thread
    command_handler();
    // does not return from above
}


