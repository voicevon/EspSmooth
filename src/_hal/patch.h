#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n );


    uint32_t min(uint32_t x, uint32_t y);

}

int strncmp ( const char * str1, const char * str2, size_t num );
void * memset ( void * ptr, int value, size_t num );