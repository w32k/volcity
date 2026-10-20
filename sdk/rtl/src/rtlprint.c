#include <rtl/format.h>
#include <rtl/string.h>



void rtl_print(const char* string);

void rtl_print_dec(const char* prev, size_t dec, bool shouldNewline){
    char buffer[20];
    rtl_dec_to_cstring(dec, buffer, 20);
    rtl_print(prev);
    rtl_print(buffer);
    if(shouldNewline) rtl_print("\n");
}

void rtl_print_hex(const char* prev, size_t dec, bool shouldNewline){
    char buffer[20];
    rtl_hex_to_cstring(dec, buffer, 20);
    rtl_print(prev);
    rtl_print("0x");
    rtl_print(buffer);
    if(shouldNewline) rtl_print("\n");
}


void rtl_printf(const char* format, ...){
    char buffer[240];
    va_list list;
    va_start(list, format);
    rtl_format(buffer, 240, format, list);
    va_end(list);
    rtl_print(buffer);
}
