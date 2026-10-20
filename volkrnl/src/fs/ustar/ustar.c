#include <fs/ustar/ustar.h>
#include <rtl/string.h>
#include <rtl/print.h>


ustarheader_t* ustar_get_header_from_memory(uintptr_t address, uintptr_t offset){
    ustarheader_t* header = (ustarheader_t*)(address + offset);
    if(rtl_equal_cstring_ex((const char*)header->uStarMagic, "ustar", 5)){
        return header;
    } else {
        return NULL;
    }
}

size_t ustar_get_filesize_from_header(ustarheader_t* header){
    return rtl_oct_string_to_size_t((const unsigned char*)header->fileSize, 11);
}

void ustar_list_all_from_memory(uintptr_t address){
    uintptr_t offset = 0;
    ustarheader_t* header = ustar_get_header_from_memory(address, offset);
    do {
        size_t fileSize = ustar_get_filesize_from_header(header);
        offset += (((fileSize + (sizeof(ustarheader_t) - 1)) / sizeof(ustarheader_t)) + 1) * sizeof(ustarheader_t);
        DEBUG_INFO("name: %s, size: %d\n", header->fileName, ustar_get_filesize_from_header(header));
        header = ustar_get_header_from_memory(address, offset);
    } while(header != NULL && rtl_equal_cstring_ex((const char*)header->uStarMagic, "ustar", 5));
}


ustarheader_t* ustar_find_file_from_memory(uintptr_t address, const char* fileName){
    uintptr_t offset = 0;
    ustarheader_t* header = ustar_get_header_from_memory(address, offset);
    do {
        if(rtl_equal_cstring((const char*)header->fileName, fileName)){
            return header;
        }
        size_t fileSize = ustar_get_filesize_from_header(header);
        offset += (((fileSize + (sizeof(ustarheader_t) - 1)) / sizeof(ustarheader_t)) + 1) * sizeof(ustarheader_t);
        header = ustar_get_header_from_memory(address, offset);
    } while(header != NULL && rtl_equal_cstring_ex((const char*)header->uStarMagic, "ustar", 5));
    return NULL;
}


uint8_t* ustar_get_filedata_from_memory(ustarheader_t* header){
    return (uint8_t*)((uintptr_t)header + sizeof(ustarheader_t));
}
