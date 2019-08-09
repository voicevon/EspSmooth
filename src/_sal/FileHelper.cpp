#include "FileHelper.h"

#include "stdio.h"

FileHelper::FileHelper(){ 
    __file_media = MEDIA_SPIFFS; 
}

void FileHelper::set_media(FILE_MEDIA_TYPE file_media){
    __file_media = file_media;
}

std::string FileHelper::test(const char* cc){
    printf("%s\n",cc);
    std::string xx ="FileHelper::test()";
    return xx;
}
std::string FileHelper::get_file_content(const char* file_name, bool show_content = false) {
    printf(" ------------------------------------ FileHelper::get_file_content()  ------------------------------------ \n");
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
    printf("SPIFFS  Total Bytes = %i \n",SPIFFS.totalBytes());
    printf("         Used Bytes = %i \n",  SPIFFS.usedBytes() );
    printf("File name = >%s<, size = %i\n\n",file_name, file.size());

    char contents[file.size()];
    file.readBytes(contents,sizeof(contents));

    // #define FILE_SIZE 17000    // the real size is 15894 Bytes,  Jun 2019

    // char file_content[FILE_SIZE];
    // int index = 0;
    // while(file.available()) {
    //     int this_byte = file.read();
    //     file_content[index] = char(this_byte);
    //     // printf ("[%i]: %i \n",index,this_byte);
    //     index++;
    //     if (index >= sizeof(file_content) - 2) break;
    // }
    // file_content[index] = 0;   //be sure to set the null terminator!!!
    // return xx;

    // std::string string_result(file_content);
    std::string string_result(contents);
    // return xx;

    // printf("[I] [spiffs.reader]:  completed Reading file %s , file size is  = %i \n",file_name, string_result.length() );
    // printf("content in type of std::string:\n  %s \n",string_result.c_str());
    // printf("==================End of content in std::string=======\n");
    // return xx;
    if(show_content){ 
        printf(" ------------------------------------ File Content >%s< ------------------------------------ \n",file_name);
        printf("%s\n", contents);
    }
    file.close();

    return string_result;

}




