#include "AdcSensors.h"

AdcSensor::AdcSensor(){

}
void AdcSensor::init(AdcPin sensor_pin,float slope, float offset){
    __sensor_pin = sensor_pin;
    _slope = slope;
    _offset = offset;
    sensor_pin.start();
}

float AdcSensor::read_adc(){
    _last_reading =  analogRead(__sensor_pin.get_gpio_id());
    return _last_reading;
}

float AdcSensor::get_meaning_value(){
    float result = _last_reading * _slope + _offset;
    return result;
}