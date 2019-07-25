#include "spiffs_ext.h"
#include "SPIFFS.h"


void test(){
    std::fstream fs;
    fs.open("/config.ini",std::fstream::in);

}

std::fstream spiffs_reading(void){
    std::fstream fs;
    
    if(!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS ");
        return fs;
    }

    File file = SPIFFS.open ("/config.ini",FILE_READ);
    if(!file) {
        Serial.println("There was an error opening the file for reading");
        return fs;
    }

    Serial.println("File content:");
    // while(file.available()) {
    //     Serial.write (file.read());
    //     //Serial.println();
    // }
    fs.open ("/config.ini",std::fstream::in);
    if(!fs.is_open()){
        Serial.write("fs open got error.");
        Serial.println("-------------------------------");
    }
    file.close();
    return fs;
}
