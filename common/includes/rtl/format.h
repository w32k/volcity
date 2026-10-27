#ifndef RTLFORMAT_H_INCLUDED
#define RTLFORMAT_H_INCLUDED

#include <voldef.h>

void rtl_print_dec(const char* prev, size_t dec, bool shouldNewline);
void rtl_print_hex(const char* prev, size_t dec, bool shouldNewline);


#include <stdarg.h>
void rtl_format(char* buf, size_t len, const char* format, va_list list);






#endif
