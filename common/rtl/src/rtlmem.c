#include <rtl/mem.h>
// slower functions, will optimize later on

void* rtl_copy_mem(const void* src, void* dest, size_t len){
    const int8_t* srcInt = src;
    int8_t* destInt = dest;
    for(size_t i = 0; i < len; i++){
        destInt[i] = srcInt[i];
    }
    return destInt;
}

void* rtl_set_mem(void* dest, uint8_t value, size_t len){
    uint8_t* destInt = dest;

    for(size_t i = 0; i < len; i++){
        destInt[i] = value;
    }
    return destInt;
}

void rtl_zero_mem(void* dest, size_t len){
    rtl_set_mem(dest, 0, len);
}

ssize_t rtl_compare_mem(const void* first, const void* second, size_t len){
    const int8_t* firstInt = first;
    const int8_t* secondInt = second;
    for(size_t i = 0; i < len; i++){
        if(firstInt[i] != secondInt[i]){
            return firstInt[i] - secondInt[i];
        }
    }
    return 0;
}


uint32_t rtl_bswap32(uint32_t b){
    return ((0xFF000000 & b) >> 24) |
        ((0x00FF0000 & b) >> 8) |
        ((0x0000FF00 & b) << 8) |
        ((0x000000FF & b) << 24);
}

uint64_t rtl_bswap64(uint64_t b){
    return ((0xFF00000000000000UL & b) >> 56) |
        ((0x00FF000000000000UL & b) >> 40) |
        ((0x0000FF0000000000UL & b) >> 24) |
        ((0x000000FF00000000UL & b) >> 8) |
        ((0x00000000FF000000UL & b) << 8) |
        ((0x0000000000FF0000UL & b) << 24) |
        ((0x000000000000FF00UL & b) << 40) |
        ((0x00000000000000FFUL & b) << 56);
}

size_t rtl_align_up(size_t value, size_t alignment){
    return ((value + alignment - 1) / alignment) * alignment;
}


// needed by most C compilers like clang
// do NOT remove these, even if we aren't using it

void* memcpy(void* dest, const void* src, size_t num){
    return rtl_copy_mem(src, dest, num);
}

void* memset(void* dest, int value, size_t len){
    return rtl_set_mem(dest, (uint8_t)value, len);
}

int memcmp(const void* ptr1, const void* ptr2, size_t len){
    return rtl_compare_mem(ptr1, ptr2, len);
}
