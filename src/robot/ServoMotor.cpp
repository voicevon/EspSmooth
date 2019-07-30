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





// const int SERVO = 18;         // Pin to control (pulse) servo
const int SERVO_CHANNEL = 0;  // 0-15
const int SERVO_FREQ = 250;    // 50Hz  20ms
const int SERVO_RES = 16;     // 16 bit resolution

const int MIN_PULSE = 1575; // 0.48mS previously 0xFFFF/20 for 1.0mS
const int MAX_PULSE = 70335; // 2.24mS previously 0xFFFF/10 for 2.0mS

ServoMotor::ServoMotor(int pin_number){
    __gpio_pin_number = pin_number;
    ledcSetup(SERVO_CHANNEL, SERVO_FREQ, SERVO_RES);
    ledcAttachPin(__gpio_pin_number,SERVO_CHANNEL);
    ledcWrite(SERVO_CHANNEL, 0);
}


long map22(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#include "HardwareSerial.h"
void ServoMotor::analogWriteESP32(int value)
{

    int val = map22(value, 0, 360, MIN_PULSE, MAX_PULSE); // Scale A/D value to servo range
    ledcWrite(SERVO_CHANNEL, val);

    Serial.print ("    ");
    Serial.print(val);
    Serial.print ("    ");
}

const int POT   = 34; // Pin to middle tap of pot

void ServoMotor::__test()
{
  int val = 0; // For storing the reading from the POT
  val = analogRead(POT); // ESP32's A/D varies from 0-4095 (12 bits) 
                         //              instead of 0-1023 (10 bits).

  val = map22(val, 0, 4095, MIN_PULSE, MAX_PULSE); // Scale A/D value to servo range
  analogWriteESP32(val); // Set the servo
}
