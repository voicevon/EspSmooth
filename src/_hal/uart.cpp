#include "uart.h"
 #include "Arduino.h"
// #include "soc/io_mux_reg.h"
// #include "esp32-hal-uart.h"  // https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-uart.h
//#include "HardwareSerial.h"
//#define UART_SERIAL_USB 0

// uart_t* Serial_USB;
// int setup_uart(){
//     // uart_t* uartBegin(uint8_t uart_nr, uint32_t baudrate, uint32_t config, int8_t rxPin, int8_t txPin, uint16_t queueLen, bool inverted);
//      Serial_USB = uartBegin(UART_SERIAL_USB, 115200, SERIAL_8N1, FUNC_U0RXD_U0RXD, FUNC_U0TXD_U0TXD, 256, false);
// }

size_t read_uart(char * buf, size_t length) {
    unsigned int  index = 0 ; 
    int incomingChar = 0;
    int rx_bytes_count = Serial.available();
    // int rx_bytes = uartAvailable(UART_SERIAL_USB);
    for (index = 0; index < rx_bytes_count; index++){
        incomingChar = Serial.read();
        buf[index] = char(incomingChar);
        // buf[counter] = uartRead(UART_SERIAL_USB);
        // index++;
    }
    return index;
}

size_t write_uart(const char * buf, size_t length){
    for(int i = 0; i < length; i++){
        Serial.print(buf[i]);
    }

    // uartWriteBuf(Serial_USB, (const uint8_t* )buf, length);
    return length;
}

void set_notification_uart(xTaskHandle h){};