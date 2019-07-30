#include "uart.h"
#include "Arduino.h"
#include "esp32-hal-uart.h"  // https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-uart.h

#define UART_SERIAL_USB 0

uart_t* Serial_USB;
int setup_uart(){
    // uart_t* uartBegin(uint8_t uart_nr, uint32_t baudrate, uint32_t config, int8_t rxPin, int8_t txPin, uint16_t queueLen, bool inverted);
    // Serial_USB = uartBegin(UART_SERIAL_USB,115200,0, )
}

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