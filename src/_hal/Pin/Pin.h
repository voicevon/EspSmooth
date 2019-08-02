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

    // enum TYPE_T {AS_INPUT, AS_OUTPUT, AS_PWM, AS_ADC};
    // Pin(const char *s, Pin::TYPE_T);

    // virtual bool init(){return false;}

    Pin* from_string(std::string value);
    std::string to_string() const;

    bool is_valid() const { return this->__valid; }
    bool connected() const { return this->__valid; }   // keep the name for SmoothieV2

    // we need to do this inline due to ISR being in SRAM not FLASH   
    // Right now, it's not in SRAM ! ForHighSpeed demand, Please call digitalRead(), and do inverting yourself.  By Xuming Jun2019
    virtual inline bool get() const
    {
        if (!this->__valid) return false;
        if(this->__gpio_id <= MAX_MCU_GPIO_INDEX) {
            bool value = digitalRead(this->__gpio_id);
            return value ^ this->inverting;
        }else { //expanded io

        }
    }

    // we need to do this inline due to ISR being in SRAM not FLASH
    // Right now, it's not in SRAM !   By Xuming Jun 2019
    virtual inline void set(bool value)
    {
        if (!this->__valid) return;
        if(this->__gpio_id <= MAX_MCU_GPIO_INDEX){
            uint8_t v= (this->inverting ^ value) ? 1 : 0;
            digitalWrite(this->__gpio_id,v);
            if(open_drain) {
                // simulates open drain by setting to input to turn off  ??
            }
        }else{  // expanded output

        }
    }

    inline uint16_t get_gpio_id() const { return this->__gpio_id; }

    bool is_inverting() const { return inverting; }
    void set_inverting(bool f) { inverting = f; }

protected:

    static bool set_allocated_pins(uint8_t, bool set= true);

    struct {
        uint8_t __gpio_id: 8;
        bool inverting: 1;
        bool open_drain: 1;
        bool __valid: 1;
        bool is_pull_up: 1;
        bool is_pull_down: 1;
        bool adc_only: 1;     //true if adc only pin
        int adc_channel: 8;   //adc channel   ??
    };
};

