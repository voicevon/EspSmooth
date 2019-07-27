#include "Pin.h"
#include "libs/StringUtils.h"

#define NUM_GPIO_PINS       99
#define NUM_GPIO_NGROUPS    2   //?

Pin::Pin()
{
    this->inverting = false;
    this->open_drain = false;
    this->valid = false;
}

Pin::Pin(const char *s)
{
    this->inverting = false;
    this->open_drain = false;
    this->valid = false;
    from_string(s);
}

Pin::Pin(const char *s, TYPE_T t)
{
    this->inverting = false;
    this->valid = false;
    this->open_drain = false;
    if(from_string(s) != nullptr) {
        switch(t) {
            case AS_INPUT: as_input(); break;
            case AS_OUTPUT: as_output(); break;
        }
    }
}

Pin::~Pin()
{

}

// bitset to indicate a port/pin has been configured
#include <bitset>
static std::bitset<256> allocated_pins;
bool Pin::set_allocated(uint8_t pin_number, bool set)
{
    return true;
    // uint8_t n = (port * NUM_GPIO_PINS) + pin;

    if(!set) {
        // deallocate it
        allocated_pins.reset(pin_number);
        return true;
    }

    if(!allocated_pins[pin_number]) {
        // if not set yet then set it
        allocated_pins.set(pin_number);
        return true;
    }

    // indicate it was already set
    return false;
}

// look up table to convert GPIO port/pin into a PINCONF
static const uint32_t port_pin_lut[NUM_GPIO_PINS] = {
   
};

// TODO: remove this function
// // given the physical port and pin (P2.7) finds the GPIO port and pin (GPIO0[7])
// static bool lookup_pin(uint16_t port, uint16_t pin, uint16_t& gpio_port, uint16_t& gpio_pin)
// {
//     for (int i = 0; i < NUM_GPIO_PORTS; ++i) {
//         for (int j = 0; j < NUM_GPIO_PINS; ++j) {
//             uint32_t v = port_pin_lut[i][j];
//             if(v == 0) continue;
//             // if( ((v & PINCONF_PINS_MASK) >> PINCONF_PINS_SHIFT) == port && ((v & PINCONF_PIN_MASK) >> PINCONF_PIN_SHIFT) == pin ) {
//             //     gpio_port = i;
//             //     gpio_pin = j;
//             //     return true;
//             // }
//         }
//     }

//     return false;
// }
#include "Arduino.h"
// Make a new pin object from a string
// Pins are defined for the LPC43xx as GPIO names GPIOp[n] or gpiop_n where p is the GPIO port and n is the pin or as pin names eg P1_6 or P1.6
// For ESP32,  string format is "gpio_xx" , for examples "gpio_03", "gpio_12",
//     For any number is greater than 35, It's an expanded io, is not a gpio on original chip.  for example, "gpio_56".
Pin* Pin::from_string(std::string value)
{
    valid = false;
    inverting = false;
    open_drain = false;

    if(value == "nc") return nullptr;

    // uint16_t port = 0;
    uint16_t pin_num = 0;
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA value = %s \n",value.c_str());

    if(stringutils::toUpper(value.substr(0, 5)) == "GPIO_") {
        // grab first integer as GPIO port.
        // port = strtol(value.substr(4).c_str(), nullptr, 10);
        String str_pos = value.substr(5,2).c_str();
        pin_num = str_pos.toInt();
        printf("Pinnumber = %i\n", pin_num);
    } else {
        return nullptr;
    }

    if(!set_allocated(pin_num)) {
        printf("WARNING: GPIO_[%d] has already been allocated\n",  pin_num);
    }

    // convert port and pin to a GPIO and setup as a GPIO
    // uint32_t gpio = port_pin_lut[pin_num];
    // if(gpio == 0) return nullptr; // not a valid pin

    // now check for modifiers:-
    // ! = invert pin
    // o = set pin to open drain
    // ^ = set pin to pull up
    // v = set pin to pull down
    // - = set pin to no pull up or down
    // default to pull up for input pins, neither for output
    // gpio |= PINCONF_PULLUP;
    for(char c : value.substr(7)) {
        switch(c) {
            case '!':
                this->inverting = true;
                break;
            case 'o':
                this->open_drain= true; // we need to set pin to input when off for simulated opendrain
                break;
            case '^':
                this->is_pull_up = true;
                this->is_pull_down = false;
                break;
            case 'v':
                this->is_pull_down = true;
                this->is_pull_up = false;
                break;
            case '-':
            	this->is_pull_up = false;
                this->is_pull_down = false;
                break;
        }
    }
    config_gpio_pin(); //configures pin for GPIO

    // save the gpio port and pin (we can always get the pin number from this and the lut)
    this->gpiopin = pin_num;
    this->valid = true;
    return this;
}

std::string Pin::to_string() const
{
    if(valid) {
        String str_pinnum = "GPIO_" + String(gpiopin);
        if(gpiopin < 10)  str_pinnum += "0";
        if(open_drain) str_pinnum += 'o'; 
        if(inverting) str_pinnum +="!";  

        std::string std_str(str_pinnum.c_str());
        return std_str;
    } else {
        return "nc";
    }
}

#include "esp32-hal-gpio.h"

Pin* Pin::as_output()
{
    if(valid) {
        //>>>Xuming
        //Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, gpioport, gpiopin);
        //Xuming<<<
        return this;
    }

    return nullptr;
}

Pin* Pin::as_input()
{
    if(valid) {
        //>>>Xuming
        //Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, gpioport, gpiopin);
        //Xuming<<<
        return this;
    }

    return nullptr;
}


bool Pin::config_gpio_pin()
{
    return true;


    int xx = 0;
    if(this->is_pull_up) xx = INPUT_PULLUP;
    if(this->is_pull_down) xx = INPUT_PULLUP;

    pinMode(this->gpiopin,xx);
    return true;
}


