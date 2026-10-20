#ifndef USTAR_H_INCLUDED
#define USTAR_H_INCLUDED








#include <types.h>



volatile typedef struct ustarheader {
    uint8_t fileName[100];
    uint64_t mode;
    uint64_t oUserId;
    uint64_t gUserId;
    uint8_t fileSize[12];
    uint8_t lastModificationDate[12];
    uint64_t hrChecksum;
    uint8_t typeFlag;
    uint8_t linkedFile[100];
    uint8_t uStarMagic[6];
    uint8_t uStarVer[2];
    uint8_t oUserName[32];
    uint8_t oGroupName[32];
    uint64_t devMajor;
    uint64_t devMinor;
    uint8_t fileNamePrefix[155];
    uint8_t padding[12];
} __attribute__((packed)) ustarheader_t;

ustarheader_t* ustar_get_header(uintptr_t address, uintptr_t offset);
size_t ustar_get_filesize_from_header(ustarheader_t* header);
void ustar_list_all_from_memory(uintptr_t address);
ustarheader_t* ustar_find_file_from_memory(uintptr_t address, const char* fileName);
uint8_t* ustar_get_filedata_from_memory(ustarheader_t* header);







#endif
