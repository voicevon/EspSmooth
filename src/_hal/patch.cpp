#include "patch.h"

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }

    uint32_t min(uint32_t x, uint32_t y){
        if(x < y) return x;
        return y;
    }

}

int strncmp ( const char * str1, const char * str2, size_t num ){
    return 0;
}

void * memset ( void * ptr, int value, size_t num ){
    void * xx;
    return xx;
}