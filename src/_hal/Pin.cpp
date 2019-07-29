#include "Pin.h"
#include "libs/StringUtils.h"


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

// bitset to indicate a pin has been configured
#include <bitset>
static std::bitset<GPIO_PINS_COUNT> allocated_pins;   //default constructor :The object is initialized with zeros.
bool Pin::__set_allocated(uint8_t pin_number, bool set)
{
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
    valid = false;
    inverting = false;
    open_drain = false;

    if(value == "nc") return nullptr;

    // uint16_t port = 0;
    uint16_t target_pin_num = 0;

    if(stringutils::toUpper(value.substr(0, 5)) == "GPIO_") {
        String str_pos = value.substr(5,2).c_str();
        target_pin_num = str_pos.toInt();
        // printf("Target Pin number = %i\n", target_pin_num);
    } else {
        return nullptr;
    }

    if(!__set_allocated(target_pin_num)) {
        printf("WARNING: GPIO_[%d] has already been allocated\n",  target_pin_num);
    }


    // save the gpio port and pin (we can always get the pin number from this and the lut)
    this->gpio_pin_num = target_pin_num;
    __from_string(value);
    this->valid = __check_validation();
    return this;
}

std::string Pin::to_string() const
{
    if(valid) {
        String str_pinnum = "GPIO_" ;
        if(this->gpio_pin_num == 0 ) str_pinnum += "0";
        if(this->gpio_pin_num < 10)  str_pinnum += "0";
        str_pinnum += String(this->gpio_pin_num);
        if(this->open_drain) str_pinnum += 'o'; 
        if(this->inverting) str_pinnum += "!";  

        std::string std_str(str_pinnum.c_str());
        return std_str;
    } else {
        return "nc";
    }
}

Pin* Pin::as_output()
{
    if(valid) {
        if(this->gpio_pin_num <= MAX_MCU_GPIO_INDEX){
            if(this->open_drain){
                pinMode(this->gpio_pin_num, OUTPUT_OPEN_DRAIN);
                return this;
            }
            pinMode(this->gpio_pin_num, OUTPUT);
        }else{  //expaned io
            
        }
    }

    return nullptr;
}

Pin* Pin::as_input()
{
    if(valid) {
        if(this->gpio_pin_num <= MAX_MCU_GPIO_INDEX ){
            if(this->is_pull_up) {
                pinMode(this->gpio_pin_num, INPUT_PULLUP);
                return this;
            } 
            if(this->is_pull_down){
                pinMode(this->gpio_pin_num, INPUT_PULLDOWN);
                return this;
            }
            pinMode(this->gpio_pin_num, INPUT);
        }else{ //expaned gpio

        }
    }
    return nullptr;
}

 bool Pin::_check_validation()
 {
     return true;
 }

 bool Pin::__check_validation()
 {
    return _check_validation();
 }
void Pin::__from_string(std::string value){
    _from_string(value);
}
void Pin::_from_string(std::string value){
    // now check for modifiers:-
    // ! = invert pin
    // o = set pin to open drain
    // p = set pin to pwm             new feature,   Xuming Jun 2019
    // h = set pin to hardware-pwm    new feature,   Xuming Jun 2019
    // ^ = set pin to pull up
    // v = set pin to pull down

    // default to pull up for input pins, neither for output   ??

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
        }
    }
}
