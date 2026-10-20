#ifndef RTLSTRING_H_INCLUDED
#define RTLSTRING_H_INCLUDED






#include <types.h>


size_t rtl_get_cstring_len(const char* src);
ssize_t rtl_compare_cstring_ex(const char* first, const char* second, size_t maxLength);
ssize_t rtl_compare_cstring(const char* first, const char* second);
bool rtl_equal_cstring_ex(const char* first, const char* second, size_t maxLength);
bool rtl_equal_cstring(const char* first, const char* second);
void rtl_copy_cstring(const char* src, char* dest, size_t len);
size_t rtl_cstring_to_dec_ex(const char* src, size_t len, bool shouldFail);
size_t rtl_cstring_to_dec(const char* src, size_t len);
size_t rtl_dec_to_cstring(const size_t number, char* src, size_t len);
size_t rtl_cstring_to_bin_ex(const char* src, size_t len, bool shouldFail);
size_t rtl_cstring_to_bin(const char* src, size_t len);
size_t rtl_bin_to_cstring(const size_t number, char* src, size_t len);
size_t rtl_cstring_to_hex_ex(const char* src, size_t len, bool shouldFail);
size_t rtl_cstring_to_hex(const char* src, size_t len);
size_t rtl_hex_to_cstring(const size_t number, char* src, size_t len);
size_t rtl_oct_string_to_size_t(const unsigned char* octStr, size_t len);





#endif
