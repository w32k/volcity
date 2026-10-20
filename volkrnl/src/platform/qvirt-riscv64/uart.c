#include <platform/qvirt-riscv64/platdef.h>
#include <platform/platform.h>




bool plat_uart_setup(){
    // will do some actual setup later
    // for now it'll just be us using the default shit
    return TRUE;
}

void plat_uart_putchar(char c){
    *(uint8_t*)UART_BASE = c;
}

char plat_uart_getchar(){
    return *(uint8_t*)UART_BASE;
}

void plat_uart_print(const char* string){
    char* s = (char*)string;
    while(*s != '\0'){
        plat_uart_putchar(*s);
        s++;
    }
}
