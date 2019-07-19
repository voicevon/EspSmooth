#include <string>
#include <sstream>

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