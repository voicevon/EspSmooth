#include <stdio.h>   //for printf()

#include "spiffs_ext.h"
#include "SPIFFS.h"

#define FILE_SIZE 17000    // the real size is 15894 Bytes,  Jun 2019

std::string spiffs_read(){
    printf(" +++++++++++++++++++++++++ \n");
    return std::string ("11111111111111");
}

std::string spiffs_read(const char* file_name){
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \n");
    delay(1000);
    printf("[D][spiffs_reading] starting...\n]");
    std::string err = "READ CONFIG ERROR!";
    
    if(!SPIFFS.begin(true)) {
        printf("An error has occurred while mounting SPIFFS \n");
        return err;
    }

    File file = SPIFFS.open (file_name, FILE_READ);
    if(!file) {
        printf("There was an error opening the file for reading\n");
        return err;
    }
    
    char file_content[FILE_SIZE];
    int index = 0;
    while(file.available()) {
        int this_byte = file.read();
        file_content[index] = char(this_byte);
        // printf ("[%i]: %i \n",index,this_byte);
        index++;
        if (index >= sizeof(file_content) - 2) break;
    }
    file_content[index] = 0;   //be sure to set the null terminator!!!

    std::string string_result(file_content);
    printf("[I] [spiffs.reader]:  completed Reading file %s , file size is  = %i \n",file_name, string_result.length() );
    // printf("content in type of std::string:\n  %s \n",string_result.c_str());
    // printf("==================End of content in std::string=======\n");

    file.close();
    return string_result;
}


void setup_spiffs_writting() {
    if(!SPIFFS.begin(true)) {
        Serial.println ("An eooro has occurred while mounting SPIFFS");
        return;
    }

    File file =  SPIFFS.open("/config.ini",FILE_WRITE);
    if(!file) {
        Serial.println("There was an error opening the file for wrtting");
        return;
    }

    if(!file.print("TEST")) {
        Serial.println("File write failed");
    }

    file.close();

}