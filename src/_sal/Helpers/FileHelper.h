#pragma once
#include "SPIFFS.h"
#include "SD.h"

class FileHelper{

    public:
        enum FILE_MEDIA_TYPE {MEDIA_SD,MEDIA_SPIFFS};
        static FileHelper* get_instance(){ return __instance; }
        ~FileHelper();

        void set_media(FILE_MEDIA_TYPE file_media);
        // std::string get_report();
        std::string get_file_content(const char* file_name, bool show_content) ;
        std::string test(const char* cc);
    private:
        FileHelper();
        FILE_MEDIA_TYPE __file_media;
        static FileHelper* __instance;

};
