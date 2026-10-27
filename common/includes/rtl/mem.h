#ifndef RTLMEM_H_INCLUDED
#define RTLMEM_H_INCLUDED

#include <voldef.h>


void* rtl_copy_mem(const void* src, void* dest, size_t len);
void* rtl_set_mem(void* dest, uint8_t value, size_t len);
void rtl_zero_mem(void* dest, size_t len);
ssize_t rtl_compare_mem(const void* first, const void* second, size_t len);
uint32_t rtl_bswap32(uint32_t b);
uint64_t rtl_bswap64(uint64_t b);
size_t rtl_align_up(size_t value, size_t alignment);




#endif
