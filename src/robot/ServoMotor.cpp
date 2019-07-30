// Exploring04-04
// Servo Potentiometer Control
//   Arduino:  LOLIN32 Lite (with ESP32)

// #include <Servo.h> This is not implemented for the ESP32, so the servo pulse has
// to be controlled directly using the analogWriteESP32() function below.

// The following array and two functions are needed because the analogWrite() function
// is not implemented for the ESP32.

#include "esp32-hal-ledc.h"
#include "esp32-hal-adc.h"
#include "ServoMotor.h"


// The default values freq=500 and resolution=8 are used for compatibility with the
// Arduino UNO, which generates PWM signals at 500Hz and 8 bit resolution.
void ServoMotor::__setupAnalogWritePin(int pin, int channel, int freq, int resolution)
{
  ledcSetup(channel, freq, resolution);
  __pin2channel[pin] = channel;
  ledcAttachPin(pin, __pin2channel[pin]);
  ledcWrite(channel, 0);
}

void ServoMotor::analogWriteESP32(int pin, int value)
{
  ledcWrite(__pin2channel[pin], value);
}

// const int SERVO = 18;         // Pin to control (pulse) servo
const int SERVO_CHANNEL = 0;  // 0-15
const int SERVO_FREQ = 50;    // 50Hz
const int SERVO_RES = 16;     // 16 bit resolution

const int MIN_PULSE = 1575; // 0.48mS previously 0xFFFF/20 for 1.0mS
const int MAX_PULSE = 7335; // 2.24mS previously 0xFFFF/10 for 2.0mS


void ServoMotor::setup_ledc(int gpio_pin_number)
{
    __setupAnalogWritePin(gpio_pin_number, SERVO_CHANNEL, SERVO_FREQ, SERVO_RES);
    __gpio_pin_number = gpio_pin_number;
}



long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

const int POT   = 34; // Pin to middle tap of pot

void ServoMotor::__test()
{
  int val = 0; // For storing the reading from the POT
  val = analogRead(POT); // ESP32's A/D varies from 0-4095 (12 bits) 
                         //              instead of 0-1023 (10 bits).

  val = map(val, 0, 4095, MIN_PULSE, MAX_PULSE); // Scale A/D value to servo range
  analogWriteESP32(__gpio_pin_number, val); // Set the servo
}
