#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED




#include <types.h>


bool plat_uart_setup();
void plat_uart_putchar(char c);
char plat_uart_getchar();
void plat_uart_print(const char* string);

#endif
