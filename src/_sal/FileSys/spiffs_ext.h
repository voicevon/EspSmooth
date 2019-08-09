#ifndef _SPIFFS_EXT_H_
#define _SPIFFS_EXT_H_

//TODO:  make a static class
#include <fstream>  //TODO:: return filestream.

std::string spiffs_read(const char* file_name);
std::string spiffs_read();

#endif
