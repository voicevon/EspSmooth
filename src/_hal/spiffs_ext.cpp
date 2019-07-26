#include "spiffs_ext.h"
#include "SPIFFS.h"


void test(){
    std::fstream fs;
    fs.open("/config.ini",std::fstream::in);

}

std::string spiffs_reading(void){

    std::string std_string;
    // printf("K000000000000\n");
    if(!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS ");
        return std_string;
    }

    File file = SPIFFS.open ("/config.ini",FILE_READ);
    if(!file) {
        Serial.println("There was an error opening the file for reading");
        return std_string;
    }
    // printf("kkkkkkkkkkkk1111111111111111\n");
    
    char ccc[200];
    int index = 0;
    printf("kkkkkkkkkkkkkkkkkk22222222222222\n");
    while(file.available()) {
        int xx = file.read();
        ccc[index] = char(xx);
        // printf ("[%i]: %i \n",index,xx);
        index++;
        if (index >= sizeof(ccc) - 2) break;
    }
    ccc[index] = 0;   //be sure to set the null terminator!!!
    // printf("K33333333333333333\n");

    String arduino_string((char*)(ccc));
    printf("Total bytes in spiffs file:  %i :\n", index);
    printf("========================Begin of content==============\n");
    Serial.println(arduino_string);
    printf("==================End of content in Arduino::String===\n");

    // std::string std_string_ =  arduino_string.c_str();
    // for(int i=0; i<100;i++){
    //     char cc = std_string_[i];
    //     printf("==[%i], %i \n",i,cc);
    // }
    // printf("in std_string, Length = %i \n",sizeof(std_string_) );
    // printf("content in type of std::string:\n  %s \n",std_string_);
    // printf("==================End of content in std::string=======\n");

    std::string ssss(ccc);
    //std::string ssss="THis is a test from                                 ddddddddddddddddd      Mars!!!!!!!!!!!!!!!!!!!!!!!!!";
    printf("in std_string, Length = %i \n",ssss.length() );
    printf("content in type of std::string:\n  %s \n",ssss.c_str());
    printf("==================End of content in std::string=======\n");


    // fs.open ("/config.ini",std::fstream::in);
    // if(!fs.is_open()){
    //     Serial.write("fs open got error.");
    //     Serial.println("-------------------------------");
    // }
    file.close();
    return std_string;
}
