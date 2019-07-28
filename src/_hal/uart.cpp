#include "uart.h"
#include "Arduino.h"

size_t read_uart(char * buf, size_t length) {
    unsigned int  counter = 0 ; 
    int incomingByte = 0;
    int rx_bytes = Serial.available();
    for (int i = 0; i < rx_bytes; i++){
        incomingByte = Serial.read();
        // Serial.print("`");
        // Serial.print(char(incomingByte));
        buf[counter] = char(incomingByte);
        counter++;
    }
    return counter;
}

size_t write_uart(const char * buf, size_t length){
    for(int i = 0; i < length; i++){
        Serial.print(buf[i]);
    }
    return length;
}

void set_notification_uart(xTaskHandle h){};