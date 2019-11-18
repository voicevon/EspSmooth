#ifndef _ADC_SENSOR_H_
#define _ADC_SENSOR_H_

class Sensor{
    protected:
        float _slope = 1.0;
        float _offset = 0.0;
        float _last_reading = 1.0;
};

#include "_HAL/Pin/AdcPin.h"

class AdcSensor: public Sensor {
    public:
        AdcSensor();
        void init(AdcPin sensor_pin,float slope, float offset);
        float read_adc();
        bool start();

        float get_meaning_value(); 
        float get_last_reading();
    private:
        AdcPin __sensor_pin;

};



#endif
