#include <rtl/string.h>
#include <rtl/mem.h>
#include <rtl/math.h>

size_t rtl_get_cstring_len(const char* src){
    size_t i = 0;
    while(*src != '\0'){
        i++;
        src++;
    }
    return i;
}

// broken
ssize_t rtl_compare_cstring_ex(const char* first, const char* second, size_t maxLength){
    for(size_t i = 0; i < maxLength; i++){
        if(first[i] != second[i]){
            return first[i] - second[i];
        }
    }
    return 0;
}

// broken
ssize_t rtl_compare_cstring(const char* first, const char* second){
    size_t firstSize = rtl_get_cstring_len(first);
    size_t secondSize = rtl_get_cstring_len(second);
    // bruh
    if(firstSize > secondSize){
        return -1;
    } else if(firstSize < secondSize){
        return 1;
    }
    return rtl_compare_cstring_ex(first, second, firstSize);
}

bool rtl_equal_cstring_ex(const char* first, const char* second, size_t maxLength){
    for(size_t i = 0; i < maxLength; i++){
        if(first[i] != second[i]){
            return FALSE;
        }
    }
    return TRUE;
}

bool rtl_equal_cstring(const char* first, const char* second){
    size_t firstSize = rtl_get_cstring_len(first);
    size_t secondSize = rtl_get_cstring_len(second);
    if(firstSize != secondSize){
        return FALSE;
    }
    return rtl_equal_cstring_ex(first, second, firstSize);
}



void rtl_copy_cstring(const char* src, char* dest, size_t len){
    for(size_t i = 0; i < len; i++){
        dest[i] = src[i];
    }
}

size_t rtl_cstring_to_dec_ex(const char* src, size_t len, bool shouldFail){
    size_t number = 0;
    for(size_t i = 0; i < len; i++){
        if(src[i] > '9' || src[i] < '0'){
            if(shouldFail) return number;
            else continue;
        }
        uint8_t singleDigit = src[i] - '0';
        number += singleDigit;
        number *= 10;
    }

    return number / 10;
}

size_t rtl_cstring_to_dec(const char* src, size_t len){
    return rtl_cstring_to_dec_ex(src, len, TRUE);
}


size_t rtl_dec_to_cstring(const size_t number, char* src, size_t len){
    if(number == 0){
        if(len >= 2){
            src[0] = '0';
            src[1] = '\0';
            return 1;
        } else return 0;
    }
    size_t digits = 0;
    size_t num = number;
    while(num > 0){
        num /= 10;
        digits++;
    }
    num = number;
    size_t index = 0;
    size_t prevDigit = digits;
    for(index = 0; index < prevDigit; index++){
        if(index >= (len - 1)){
            break;
        }
        size_t amount = rtl_pow(10, digits - index);
        uint8_t digit = num / amount;
        src[index] = digit + '0';
        num -= (amount * digit);
    }
    if(index == 0 && index < (len - 1)){
        src[index] = '0';
    }
    src[index] = '\0';
    return digits;
}

// broken
size_t rtl_cstring_to_bin_ex(const char* src, size_t len, bool shouldFail){
    size_t number = 0;
    for(size_t i = 0; i < len; i++){
        if(src[i] > '1' || src[i] < '0'){
            if(shouldFail) return number;
            else continue;
        }
        uint8_t singleDigit = src[i] - '0';
        number += singleDigit;
        number *= 2;
    }

    return number / 2;
}

size_t rtl_cstring_to_bin(const char* src, size_t len){
    return rtl_cstring_to_bin_ex(src, len, TRUE);
}

size_t rtl_bin_to_cstring(const size_t number, char* src, size_t len){
    if(number == 0){
        if(len >= 2){
            src[0] = '0';
            src[1] = '\0';
            return 1;
        } else return 0;
    }
    size_t digits = 0;
    size_t num = number;
    while(num > 0){
        num /= 2;
        digits++;
    }
    num = number;
    size_t index = 0;
    size_t prevDigit = digits;
    for(index = 0; index < prevDigit; index++){
        if(index >= (len - 1)){
            break;
        }
        size_t amount = rtl_pow(2, digits - index);
        uint8_t digit = num / amount;
        src[index] = digit + '0';
        num -= (amount * digit);
    }
    if(index == 0 && index < (len - 1)){
        src[index] = '0';
    }
    src[index] = '\0';
    return digits;
}

static inline bool char_is_hex(char c){
    if(c <= '9' && c >= '0'){
        return TRUE;
    } else if(c <= 'F' && c >= 'A'){
        return TRUE;
    } else if(c <= 'f' && c >= 'a'){
        return TRUE;
    } else return FALSE;
}

static inline uint8_t char_to_hex(char c){
    switch(c){
        case 'a':
        case 'A':
            return 0xA;
        case 'b':
        case 'B':
            return 0xB;
        case 'c':
        case 'C':
            return 0xC;
        case 'd':
        case 'D':
            return 0xD;
        case 'e':
        case 'E':
            return 0xE;
        case 'f':
        case 'F':
            return 0xF;
        default:
            return 0x0;
    }
}

static inline char hex_to_char(uint8_t h){
    switch(h){
        case 0xA:
            return 'A';
        case 0xB:
            return 'B';
        case 0xC:
            return 'C';
        case 0xD:
            return 'D';
        case 0xE:
            return 'E';
        case 0xF:
            return 'F';
        default:
            return '?';
    }
}

size_t rtl_cstring_to_hex_ex(const char* src, size_t len, bool shouldFail){
    size_t number = 0;
    for(size_t i = 0; i < len; i++){
        if(!char_is_hex(src[i])){
            if(shouldFail) return number;
            else continue;
        }
        uint8_t singleDigit = 0;
        if(src[i] <= '9' && src[i] >= '0'){
            singleDigit = src[i] - '0';
        } else singleDigit = char_to_hex(src[i]);
        number += singleDigit;
        number *= 16;
    }

    return number / 16;
}

size_t rtl_cstring_to_hex(const char* src, size_t len){
    return rtl_cstring_to_hex_ex(src, len, TRUE);
}


size_t rtl_hex_to_cstring(const size_t number, char* src, size_t len){
    if(number == 0x0){
        if(len >= 2){
            src[0] = '0';
            src[1] = '\0';
            return 1;
        } else return 0;
    }
    size_t digits = 0;
    size_t num = number;
    while(num > 0){
        num /= 16;
        digits++;
    }
    num = number;
    size_t index = 0;
    size_t prevDigit = digits;
    for(index = 0; index < prevDigit; index++){
        if(index >= (len - 1)){
            break;
        }
        size_t amount = rtl_pow(16, digits - index);
        uint8_t digit = num / amount;
        if(digit <= 0x9 && digit >= 0x0){
            src[index] = digit + '0';
        } else src[index] = hex_to_char(digit);
        num -= (amount * digit);
    }
    if(index == 0 && index < (len - 1)){
        src[index] = '0';
        index++;
    }
    src[index] = '\0';
    return digits;
}

size_t rtl_oct_string_to_size_t(const unsigned char* octStr, size_t len){
    size_t result = 0;
    unsigned char* str = (unsigned char*)octStr;
    while(len > 0){
        len--;
        result *= 8;
        result += *str - '0';
        str++;
    }
    return result;
}
