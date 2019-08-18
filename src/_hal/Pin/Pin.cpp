#include "Pin.h"
#include "libs/StringUtils.h"
#include "HardwareSerial.h"

Pin::Pin()
{
    this->inverting_ = false;
    this->open_drain = false;
    this->valid_ = false;
}

Pin::Pin(const char *s)
{
    this->inverting_ = false;
    this->open_drain = false;
    this->valid_ = false;
    from_string(s);
}


Pin::~Pin()
{

}

// bitset to indicate a pin has been configured
#include <bitset>
static std::bitset<GPIO_PINS_COUNT> allocated_pins;   //default constructor :The object is initialized with zeros.
bool Pin::set_allocated_pins_(uint8_t pin_number, bool set)
{
    // printf("dddddddddddddddd %d\n",allocated_pins);
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

#include "WString.h"
// Make a new pin object from a string
// Pins are defined for the LPC43xx as GPIO names GPIOp[n] or gpiop_n where p is the GPIO port and n is the pin or as pin names eg P1_6 or P1.6
// For ESP32,  string format is "gpio_xx" , for examples "gpio_03", "gpio_12",
//     For any number is greater than 35, It's an expanded io, is not a gpio on original chip.  for example, "gpio_56".
Pin* Pin::from_string(std::string value)
{
    valid_ = false;
    inverting_ = false;
    open_drain = false;

    if(value == "nc") return nullptr;

    // uint16_t port = 0;
    uint16_t target_pin_id = 0;

    if(stringutils::toUpper(value.substr(0, 5)) == "GPIO_") {
        String str_pos = value.substr(5,2).c_str();
        target_pin_id = str_pos.toInt();
        // printf("Target Pin number = %i\n", target_pin_num);
    } else {
        return nullptr;
    }

    if(!set_allocated_pins_(target_pin_id)) {
        printf("WARNING: GPIO_[%d] has already been allocated\n",  target_pin_id);
    }


    // now check for modifiers:-
    // ! = invert pin
    // o = set pin to open drain
    // ^ = set pin to pull up
    // v = set pin to pull down
    // - = set pin to no pull up or down
    // default to pull up for input pins, neither for output
    // gpio |= PINCONF_PULLUP;

    // f = pwm frequency
    // a = adc gain db
    // c = pulse counter
    for(char c : value.substr(7)) {
        switch(c) {
            case '!':
                this->inverting_ = true;
                break;
            case 'o':
                this->open_drain= true; // we need to set pin to input when off for simulated opendrain
                break;
            case '^':
                this->is_pull_up_ = true;
                this->is_pull_down_ = false;
                break;
            case 'v':
                this->is_pull_down_ = true;
                this->is_pull_up_ = false;
                break;
            case '-':
            	this->is_pull_up_ = false;
                this->is_pull_down_ = false;
                break;
        }
    }

    // save the gpio port and pin (we can always get the pin number from this and the lut)
    this->gpio_id_ = target_pin_id;
    this->valid_ = true;
    return this;
}

std::string Pin::to_string() const
{
    if(valid_) {
        String str_pinnum = "GPIO_" ;
        if(this->gpio_id_ == 0 ) str_pinnum += "0";
        if(this->gpio_id_ < 10)  str_pinnum += "0";
        str_pinnum += String(this->gpio_id_);
        if(this->open_drain) str_pinnum += 'o'; 
        if(this->inverting_) str_pinnum += "!";  

        std::string std_str(str_pinnum.c_str());
        return std_str;
    } else {
        return "nc";
    }
}








