#pragma once


//  Limitation of ESP32 GPIO
//  https://randomnerdtutorials.com/esp32-pinout-reference-gpios/


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <bitset>

#include "esp32-hal-gpio.h"

#define GPIO_PINS_COUNT      99    //All IO pins, include expanded
#define MAX_MCU_GPIO_INDEX   35

#if(GPIO_PINS_COUNT > MAX_MCU_GPIO_INDEX) 
    #define USE_EXPANDED_IO
#endif

class Pin
{
public:
    virtual ~Pin();

    Pin* from_string(std::string value);   //should be a protected member? Can NOT do it directly!
    std::string to_string() const;

    virtual bool start(){ return false; }
    virtual bool stop(){ return false; }

    bool is_valid() const { return this->valid_; }
    bool connected() const { return this->valid_; }   // keep the name for SmoothieV2

    // we need to do this inline due to ISR being in SRAM not FLASH   
    // Right now, it's not in SRAM ! ForHighSpeed demand, Please call digitalRead(), and do inverting yourself.  By Xuming Jun2019
    virtual inline bool get() const
    {
        if (!this->valid_) return false;
        if(this->gpio_id_ <= MAX_MCU_GPIO_INDEX) {
            bool value = digitalRead(this->gpio_id_);
            return value ^ this->inverting_;
        }else { //expanded io

        }
        return false;
    }

    // we need to do this inline due to ISR being in SRAM not FLASH
    // Right now, it's not in SRAM !   By Xuming Jun 2019
    virtual inline void set(bool value)
    {
        if (!this->valid_) return;
        if(this->gpio_id_ <= MAX_MCU_GPIO_INDEX){
            uint8_t v= (this->inverting_ ^ value) ? 1 : 0;
            digitalWrite(this->gpio_id_,v);
            if(open_drain) {
                // simulates open drain by setting to input to turn off  ??
            }
        }else{  // expanded output

        }
    }

    inline uint8_t get_gpio_id() const { return this->gpio_id_; }

    bool is_inverting() const { return inverting_; }
    void set_inverting(bool f) { inverting_ = f; }

protected:
    Pin();
    Pin(const char *s);

    static bool set_allocated_pins_(uint8_t, bool set= true);

    struct {
        uint8_t gpio_id_: 8;
        bool inverting_: 1;
        bool open_drain: 1;
        bool valid_: 1;
        bool is_pull_up: 1;
        bool is_pull_down: 1;
        bool adc_only: 1;     //true if adc only pin
        int adc_channel: 8;   //adc channel   ??
    };
};

