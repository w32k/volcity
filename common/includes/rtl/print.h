#ifndef RTLPRINT_H_INCLUDED
#define RTLPRINT_H_INCLUDED






#include <voldef.h>


void rtl_print(const char* string);
void rtl_print_dec(const char* prev, size_t dec, bool shouldNewline);
void rtl_print_hex(const char* prev, size_t dec, bool shouldNewline);
void rtl_printf(const char* format, ...);

#define DEBUG_FAIL(message, ...) rtl_printf("[FAIL] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_PASS(message, ...) rtl_printf("[PASS] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_INFO(message, ...) rtl_printf("[INFO] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_WARNING(message, ...) rtl_printf("[WARNING] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)










#endif
