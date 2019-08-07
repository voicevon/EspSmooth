#include "version.h"
const char *Version::get_build(void) const {
    #define  __GITVERSIONSTRING__  "ESP32 Version 0.1"
    #ifdef DISABLEMSD
        return __GITVERSIONSTRING__ "NOMSD";
    #else
        return __GITVERSIONSTRING__;
    #endif
}
const char *Version::get_build_date(void) const {
    return __DATE__ " " __TIME__;
}
