#include "uart.h"
#include "HardwareSerial.h"

size_t read_uart(char * buf, size_t length) {
    unsigned int  index = 0 ; 
    int incomingChar = 0;
    int rx_bytes_count = Serial.available();
    for (index = 0; index < rx_bytes_count; index++){
        incomingChar = Serial.read();
        buf[index] = char(incomingChar);
    }
    return index;
}

size_t write_uart(const char * buf, size_t length){
    for(int i = 0; i < length; i++){
        Serial.print(buf[i]);
    }
    return length;
}

void set_notification_uart(xTaskHandle h){};