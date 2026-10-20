#ifndef DEVDT_H_INCLUDED
#define DEVDT_H_INCLUDED







#include <device/device.h>



#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE 0x00000002
#define FDT_PROP 0x00000003
#define FDT_NOP 0x00000004
#define FDT_END 0x00000009

typedef struct _FdtHeader {
    uint32_t magic;
    uint32_t totalSize;
    uint32_t dtStructOffset;
    uint32_t dtStringsOffset;
    uint32_t memReservOffset;
    uint32_t version;
    uint32_t lastCompVersion;
    uint32_t bootPhysCpuid;
    uint32_t dtStringSize;
    uint32_t dtStructSize;
} FdtHeader;

typedef struct _FdtInfo {
    FdtHeader header;
    uint32_t* cells;
    size_t cellAmount;
    uintptr_t fdtAddress;
} FdtInfo;

typedef struct _FdtProp {
    uint32_t length;
    uint32_t nameOffset;
} FdtProp;


bool fdt_init(FdtInfo* info, uintptr_t address);
const char* fdt_get_string(FdtInfo* info, uint32_t offset);
void fdt_print_all_strings(FdtInfo* info);
void fdt_print_node(FdtInfo* info, uint32_t offset);
void fdt_dump(FdtInfo* info);
bool fdt_find_node(FdtInfo* info, const char* nodeName, uint32_t* offset);
FdtProp* fdt_get_prop_ex(FdtInfo* info, uint32_t nodeOffset, const char* propName, uint32_t* propOffset);
FdtProp* fdt_get_prop(FdtInfo* info, uint32_t nodeOffset, const char* propName);
uint32_t fdt_get_value_from_prop(FdtInfo* info, uint32_t propOffset);
void fdt_get_array_from_prop(FdtInfo* info, uint32_t propOffset, uintptr_t* array, size_t length);
//const char* fdt_get_string_from_prop(FdtInfo* info, uint32_t propOffset);









#endif
