# include "espsmooth.h"
#include "libs/Kernel.h"

using namespace espsmooth;

endstop::EndstopSensor * endstop_x;
endstop::EndstopSensor * temperature_hotend;


void smooth_init() {

    Kernel* kernel = new Kernel();

    kernel->streams->printf("Smoothie Running @%ldMHz\r\n", SystemCoreClock / 1000000);
    SimpleShell::version_command("", kernel->streams);

    bool sdok= (sd.disk_initialize() == 0);
    if(!sdok) kernel->streams->printf("SDCard failed to initialize\r\n");

    #ifdef NONETWORK
        kernel->streams->printf("NETWORK is disabled\r\n");
    #endif

#ifdef DISABLEMSD
    // attempt to be able to disable msd in config
    if(sdok && !kernel->config->value( disable_msd_checksum )->by_default(true)->as_bool()){
        // HACK to zero the memory USBMSD uses as it and its objects seem to not initialize properly in the ctor
        size_t n= sizeof(USBMSD);
        void *v = AHB0.alloc(n);
        memset(v, 0, n); // clear the allocated memory
        msc= new(v) USBMSD(&u, &sd); // allocate object using zeroed memory
    }else{
        msc= NULL;
        kernel->streams->printf("MSD is disabled\r\n");
    }
#endif

    // Create and add main modules
    kernel->add_module( new(AHB0) Player() );

    kernel->add_module( new(AHB0) CurrentControl() );
    kernel->add_module( new(AHB0) KillButton() );
    kernel->add_module( new(AHB0) PlayLed() );

    // these modules can be completely disabled in the Makefile by adding to EXCLUDE_MODULES
    #ifndef NO_TOOLS_SWITCH
    SwitchPool *sp= new SwitchPool();
    sp->load_tools();
    delete sp;
    #endif
    #ifndef NO_TOOLS_EXTRUDER
    // NOTE this must be done first before Temperature control so ToolManager can handle Tn before temperaturecontrol module does
    ExtruderMaker *em= new ExtruderMaker();
    em->load_tools();
    delete em;
    #endif
    #ifndef NO_TOOLS_TEMPERATURECONTROL
    // Note order is important here must be after extruder so Tn as a parameter will get executed first
    TemperatureControlPool *tp= new TemperatureControlPool();
    tp->load_tools();
    delete tp;
    #endif
    #ifndef NO_TOOLS_ENDSTOPS
    kernel->add_module( new(AHB0) Endstops() );
    #endif
    #ifndef NO_TOOLS_LASER
    kernel->add_module( new Laser() );
    #endif
    #ifndef NO_TOOLS_SPINDLE
    SpindleMaker *sm= new SpindleMaker();
    sm->load_spindle();
    delete sm;
    //kernel->add_module( new(AHB0) Spindle() );
    #endif
    #ifndef NO_UTILS_PANEL
    kernel->add_module( new(AHB0) Panel() );
    #endif
    #ifndef NO_TOOLS_ZPROBE
    kernel->add_module( new(AHB0) ZProbe() );
    #endif
    #ifndef NO_TOOLS_SCARACAL
    kernel->add_module( new(AHB0) SCARAcal() );
    #endif
    #ifndef NO_TOOLS_ROTARYDELTACALIBRATION
    kernel->add_module( new(AHB0) RotaryDeltaCalibration() );
    #endif
    #ifndef NONETWORK
    kernel->add_module( new Network() );
    #endif
    #ifndef NO_TOOLS_TEMPERATURESWITCH
    // Must be loaded after TemperatureControl
    kernel->add_module( new(AHB0) TemperatureSwitch() );
    #endif
    #ifndef NO_TOOLS_DRILLINGCYCLES
    kernel->add_module( new(AHB0) Drillingcycles() );
    #endif
    #ifndef NO_TOOLS_FILAMENTDETECTOR
    kernel->add_module( new(AHB0) FilamentDetector() );
    #endif
    #ifndef NO_UTILS_MOTORDRIVERCONTROL
    kernel->add_module( new MotorDriverControl(0) );
    #endif
    // Create and initialize USB stuff
    u.init();

#ifdef DISABLEMSD
    if(sdok && msc != NULL){
        kernel->add_module( msc );
    }
#else
    kernel->add_module( &msc );
#endif

    kernel->add_module( &usbserial );
    if( kernel->config->value( second_usb_serial_enable_checksum )->by_default(false)->as_bool() ){
        kernel->add_module( new(AHB0) USBSerial(&u) );
    }

    if( kernel->config->value( dfu_enable_checksum )->by_default(false)->as_bool() ){
        kernel->add_module( new(AHB0) DFU(&u));
    }

    // 10 second watchdog timeout (or config as seconds)
    float t= kernel->config->value( watchdog_timeout_checksum )->by_default(10.0F)->as_number();
    if(t > 0.1F) {
        // NOTE setting WDT_RESET with the current bootloader would leave it in DFU mode which would be suboptimal
        kernel->add_module( new Watchdog(t*1000000, WDT_MRI)); // WDT_RESET));
        kernel->streams->printf("Watchdog enabled for %f seconds\n", t);
    }else{
        kernel->streams->printf("WARNING Watchdog is disabled\n");
    }


    kernel->add_module( &u );

    // memory before cache is cleared
    //SimpleShell::print_mem(kernel->streams);

    // clear up the config cache to save some memory
    kernel->config->config_cache_clear();

    if(kernel->is_using_leds()) {
        // set some leds to indicate status... led0 init done, led1 mainloop running, led2 idle loop running, led3 sdcard ok
        leds[0]= 1; // indicate we are done with init
        leds[3]= sdok?1:0; // 4th led indicates sdcard is available (TODO maye should indicate config was found)
    }

    if(sdok) {
        // load config override file if present
        // NOTE only Mxxx commands that set values should be put in this file. The file is generated by M500
        FILE *fp= fopen(kernel->config_override_filename(), "r");
        if(fp != NULL) {
            char buf[132];
            kernel->streams->printf("Loading config override file: %s...\n", kernel->config_override_filename());
            while(fgets(buf, sizeof buf, fp) != NULL) {
                kernel->streams->printf("  %s", buf);
                if(buf[0] == ';') continue; // skip the comments
                struct SerialMessage message= {&(StreamOutput::NullStream), buf};
                kernel->call_event(ON_CONSOLE_LINE_RECEIVED, &message);
            }
            kernel->streams->printf("config override file executed\n");
            fclose(fp);
        }
    }

    // start the timers and interrupts
    THEKERNEL->conveyor->start(THEROBOT->get_number_registered_motors());
    THEKERNEL->step_ticker->start();
    THEKERNEL->slow_ticker->start();
}

void setup(){
    endstop_x = new endstop::EndstopSensor();
    App.register_component(endstop_x);

    temperature_hotend = new endstop::EndstopSensor();
    App.register_component(temperature_hotend);

    App.setup();    
}

void loop(){
    App.loop();
}