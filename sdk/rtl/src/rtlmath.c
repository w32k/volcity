#include <rtl/math.h>
// very unoptimized =(


size_t rtl_pow(size_t base, size_t exp){
    size_t result = 1;
    for(size_t i = 0; i < (exp - 1); i++){
        result *= base;
    }
    return result;
}
