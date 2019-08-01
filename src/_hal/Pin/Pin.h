#pragma once

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
    Pin();
    Pin(const char *s);
    virtual ~Pin();

    enum TYPE_T {AS_INPUT, AS_OUTPUT, AS_PWM, AS_ADC};
    // Pin(const char *s, Pin::TYPE_T);

    virtual bool init(){return false;}

    Pin* from_string(std::string value);
    std::string to_string() const;

    bool connected() const
    {
        return this->valid;
    }



    // we need to do this inline due to ISR being in SRAM not FLASH   
    // Right now, it's not in SRAM ! ForHighSpeed demand, Please call digitalRead(), and do inverting yourself.  By Xuming Jun2019
    virtual inline bool get() const
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
    virtual inline void set(bool value)
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

    inline uint16_t get_gpio_num() const { return this->gpio_pin_num; }

    bool is_inverting() const { return inverting; }
    void set_inverting(bool f) { inverting = f; }

protected:

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

