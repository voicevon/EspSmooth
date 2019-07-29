#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <bitset>

#include "esp32-hal-gpio.h"

#define MAX_MCU_GPIO_INDEX   35

class Pin
{
public:
    Pin();
    Pin(const char *s);
    virtual ~Pin();

    enum TYPE_T {AS_INPUT, AS_OUTPUT};
    Pin(const char *s, Pin::TYPE_T);


    Pin* from_string(std::string value);
    std::string to_string() const;

    bool connected() const
    {
        return this->valid;
    }

    Pin* as_output();
    Pin* as_input();

    // we need to do this inline due to ISR being in SRAM not FLASH   
    // Right now, it's not in SRAM !
    inline bool get() const
    {
        if (!this->valid) return false;
        if(this->gpio_pin_num <= MAX_MCU_GPIO_INDEX) {
            bool value = digitalRead(this->gpio_pin_num);
            return value ^ this->inverting;
        }else { //expanded io

        }

    }

    // we need to do this inline due to ISR being in SRAM not FLASH
    // Right now, it's not in SRAM !   By Xuming Jun 2019
    inline void set(bool value)
    {
        if (!this->valid) return;
        if(this->gpio_pin_num <= MAX_MCU_GPIO_INDEX){
            uint8_t v= (this->inverting ^ value) ? 1 : 0;
            digitalWrite(this->gpio_pin_num,v);
            if(open_drain) {
                // simulates open drain by setting to input to turn off  ??
            }
        }else{  // expanded output

        }
    }

    inline uint16_t get_gpiopin() const { return this->gpio_pin_num; }

    bool is_inverting() const { return inverting; }
    void set_inverting(bool f) { inverting = f; }

private:

    static bool set_allocated(uint8_t, bool set= true);

    struct {
        uint8_t gpio_pin_num: 8;
        bool inverting: 1;
        bool open_drain: 1;
        bool valid: 1;
        bool is_pull_up: 1;
        bool is_pull_down: 1;
        bool adc_only: 1;     //true if adc only pin
        int adc_channel: 8;   //adc channel   ??
    };
};

