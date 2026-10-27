#include <rtl/format.h>
#include <rtl/string.h>

#include <stdarg.h>

void rtl_format(char* buf, size_t len, const char* format, va_list list){
    size_t bufIndex = 0;
    while(*format != '\0' && bufIndex < len){
        if(*format == '%'){
            bool isLong = FALSE;
            format++;
            char nextChar = *format;
            if(nextChar == '\0') break;
            if(nextChar == 'l'){
                isLong = TRUE;
                format++;
                nextChar = *format;
                if(nextChar == '\0') break;
            }
            switch(nextChar){
                case 'c':{
                    char c = (char)(va_arg(list, int));
                    buf[bufIndex] = c;
                    bufIndex++;
                    break;
                }
                case 'd':{
                    char buffer[22];
                    size_t num = 0;
                    if(isLong == TRUE){
                        num = va_arg(list, int64_t);
                    } else num = va_arg(list, int32_t);
                    size_t digits = rtl_dec_to_cstring(num, buffer, 22);
                    for(size_t i = 0; i < digits; i++){
                        buf[bufIndex] = buffer[i];
                        bufIndex++;
                        if(bufIndex >= len){
                            break;
                        }
                    }
                    break;
                }
                case 'u':{
                    char buffer[22];
                    size_t num = 0;
                    if(isLong == TRUE){
                        num = va_arg(list, uint64_t);
                    } else num = va_arg(list, uint32_t);
                    size_t digits = rtl_dec_to_cstring(num, buffer, 22);
                    for(size_t i = 0; i < digits; i++){
                        buf[bufIndex] = buffer[i];
                        bufIndex++;
                        if(bufIndex >= len){
                            break;
                        }
                    }
                    break;
                }
                case 'b':{
                    char buffer[22];
                    size_t num = 0;
                    if(isLong == TRUE){
                        num = va_arg(list, uint64_t);
                    } else num = va_arg(list, uint32_t);
                    size_t digits = rtl_bin_to_cstring(num, buffer, 22);
                    for(size_t i = 0; i < digits; i++){
                        buf[bufIndex] = buffer[i];
                        bufIndex++;
                        if(bufIndex >= len){
                            break;
                        }
                    }
                    break;
                }
                case 'x':{
                    char buffer[22];
                    size_t num = 0;
                    if(isLong == TRUE){
                        num = va_arg(list, uint64_t);
                    } else num = va_arg(list, uint32_t);
                    size_t digits = rtl_hex_to_cstring(num, buffer, 22);
                    for(size_t i = 0; i < digits; i++){
                        buf[bufIndex] = buffer[i];
                        bufIndex++;
                        if(bufIndex >= len){
                            break;
                        }
                    }
                    break;
                }
                case 's':{
                    char* str = va_arg(list, char*);
                    while(*str != '\0'){
                        buf[bufIndex] = *str;
                        bufIndex++;
                        if(bufIndex >= len){
                            break;
                        }
                        str++;
                    }
                }
                default:
                    break;
            }
            format++;
        } else {
            buf[bufIndex] = *format;
            bufIndex++;
            format++;
        }
    }
    buf[bufIndex] = '\0';
}
