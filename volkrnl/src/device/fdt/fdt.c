#include <device/fdt/fdt.h>
#include <rtl/mem.h>
#include <rtl/print.h>
#include <rtl/string.h>






bool fdt_init(fdtinfo_t* info, uintptr_t address){
    fdtheader_t* initial = (fdtheader_t*)address;
    info->fdtAddress = address;
    info->header.magic = rtl_bswap32(initial->magic);
    if(info->header.magic != 0xd00dfeed){
        DEBUG_FAIL("header magic is not 0xd00dfeed, its 0x%x\n", info->header.magic);
        return FALSE;
    }
    info->header.totalSize = rtl_bswap32(initial->totalSize);
    info->header.dtStructOffset = rtl_bswap32(initial->dtStructOffset);
    info->header.dtStringsOffset = rtl_bswap32(initial->dtStringsOffset);
    info->header.memReservOffset = rtl_bswap32(initial->memReservOffset);
    info->header.version = rtl_bswap32(initial->version);
    info->header.lastCompVersion = rtl_bswap32(initial->lastCompVersion);
    info->header.bootPhysCpuid = rtl_bswap32(initial->bootPhysCpuid);
    info->header.dtStringSize = rtl_bswap32(initial->dtStringSize);
    info->header.dtStructSize = rtl_bswap32(initial->dtStructSize);
    info->cells = (uint32_t*)(info->fdtAddress + info->header.dtStructOffset);
    info->cellAmount = info->header.dtStructSize / sizeof(uint32_t);
    return TRUE;
}


const char* fdt_get_string(fdtinfo_t* info, uint32_t offset){
    if(offset >= info->header.dtStringSize){
        return "invalid";
    }
    uintptr_t address = info->fdtAddress + info->header.dtStringsOffset;
    const char* string = (const char*)(address + offset);
    return string;
}

void fdt_print_all_strings(fdtinfo_t* info){
    uintptr_t offset = 0;
    const char* style = "------------------------------------------------------------\n";
    rtl_print("\n");
    rtl_print(style);
    rtl_print("printing fdt string table\n");
    rtl_print(style);
    while(offset < info->header.dtStringSize){
        const char* string = fdt_get_string(info, offset);
        rtl_print(string);
        rtl_print("\n");
        size_t len = rtl_get_cstring_len(string);
        offset += len + 1;

    }
    rtl_print(style);
    rtl_print("\n");
}




void fdt_print_node(fdtinfo_t* info, uint32_t offset){
    while(offset < info->cellAmount){
        uint32_t value = rtl_bswap32(info->cells[offset]);
        switch(value){
            case FDT_BEGIN_NODE:{
                rtl_print("FDT_BEGIN_NODE\n");
                const char* name = (const char*)(info->cells + offset + 1);
                rtl_print(name);
                rtl_print("\n");
                size_t len = rtl_get_cstring_len(name);
                offset += (rtl_align_up(len + 1, 4) / 4) + 1;
                break;
            }
            case FDT_END_NODE:{
                rtl_print("FDT_END_NODE\n");
                return;
                break;
            }
            case FDT_PROP:{
                rtl_print("\tFDT_PROP\n");
                offset++;
                fdtprop_t* prop = (fdtprop_t*)(info->cells + offset);
                rtl_printf("\t\t%s, %d, 0x%x\n",
                        fdt_get_string(info, rtl_bswap32(prop->nameOffset)),
                        rtl_bswap32(prop->length), rtl_bswap32(info->cells[offset + 2]));
                offset += (rtl_align_up(rtl_bswap32(prop->length), 4) / 4)  + 2;
                break;
            }
            default:{
                rtl_printf("\t\tunknown data at %d (index is %d)\n", offset, value);
                offset+=2;
            }
        }
    }
}

// absoloutly horrendous code
// its also very unsafe, TODO: make it safe!!!
void fdt_print_node_and_step(fdtinfo_t* info, uint32_t* offset){
    while(*offset < info->cellAmount){
        uint32_t value = rtl_bswap32(info->cells[*offset]);
        switch(value){
            case FDT_BEGIN_NODE:{
                rtl_print("FDT_BEGIN_NODE\n");
                const char* name = (const char*)(info->cells + *offset + 1);
                rtl_print(name);
                rtl_print("\n");
                size_t len = rtl_get_cstring_len(name);
                *offset += (rtl_align_up(len + 1, 4) / 4) + 1;
                break;
            }
            case FDT_END_NODE:{
                rtl_print("FDT_END_NODE\n");
                return;
                break;
            }
            case FDT_PROP:{
                rtl_print("\tFDT_PROP\n");
                *offset += 1;;
                fdtprop_t* prop = (fdtprop_t*)(info->cells + *offset);
                rtl_printf("\t\t%s, %d, 0x%x\n",
                        fdt_get_string(info, rtl_bswap32(prop->nameOffset)),
                        rtl_bswap32(prop->length), rtl_bswap32(info->cells[*offset + 2]));
                *offset += (rtl_align_up(rtl_bswap32(prop->length), 4) / 4)  + 2;
                break;
            }
            default:{
                rtl_printf("\t\tunknown data at %d (index is %d)\n", *offset, value);
                *offset+=2;
            }
        }
    }
}


// its a mess and it doesn't even work, gotta fix it
void fdt_dump(fdtinfo_t* info){
    const char* style = "------------------------------------------------------------\n";
    rtl_print("\n");
    rtl_print(style);
    for(uint32_t i = 0; i < info->cellAmount; i++){
        if(rtl_bswap32(info->cells[i]) != FDT_BEGIN_NODE) continue;
        rtl_printf("node found at %d!\n", i);
        fdt_print_node_and_step(info, &i);

    }
    rtl_print(style);
    rtl_print("\n");
}

bool fdt_find_node(fdtinfo_t* info, const char* nodeName, uint32_t* nodeOffset){
    for(uint32_t i = 0; i < info->cellAmount; i++){
        if(rtl_bswap32(info->cells[i]) != FDT_BEGIN_NODE) continue;
        const char* name = (const char*)(info->cells + i + 1);
        if(rtl_equal_cstring(name, nodeName)){
            *nodeOffset = i;
            return TRUE;
        } else {
            size_t len = rtl_get_cstring_len(name);
            i += (rtl_align_up(len + 1, 4) / 4) + 1;
        }
    }
    *nodeOffset = 0;
    return FALSE;
}

fdtprop_t* fdt_get_prop_ex(fdtinfo_t* info, uint32_t nodeOffset, const char* propName, uint32_t* propOffset){
    uint32_t offset = nodeOffset;
    while(offset < info->cellAmount){
        uint32_t value = rtl_bswap32(info->cells[offset]);
        switch(value){
            case FDT_BEGIN_NODE:{
                const char* name = (const char*)(info->cells + offset + 1);
                size_t len = rtl_get_cstring_len(name);
                offset += (rtl_align_up(len + 1, 4) / 4) + 1;
                break;
            }
            case FDT_END_NODE:{
                *propOffset = 0;
                return NULL;
                break;
            }
            case FDT_PROP:{
                offset++;
                fdtprop_t* prop = (fdtprop_t*)(info->cells + offset);
                const char* name = fdt_get_string(info, rtl_bswap32(prop->nameOffset));
                if(rtl_equal_cstring(propName, name)){
                    *propOffset = offset;
                    return prop;
                }
                offset += (rtl_align_up(rtl_bswap32(prop->length), 4) / 4)  + 2;
                break;
            }
            default:{
                rtl_printf("\t\tunknown data at %d (index is %d)\n", offset, value);
                offset+=2;
            }
        }
    }
    *propOffset = 0;
    return NULL;
}

fdtprop_t* fdt_get_prop(fdtinfo_t* info, uint32_t nodeOffset, const char* propName){
    uint32_t pOffset = 0;
    return fdt_get_prop_ex(info, nodeOffset, propName, &pOffset);
}


uint32_t fdt_get_value_from_prop(fdtinfo_t* info, uint32_t propOffset){
    return rtl_bswap32(info->cells[propOffset + 2]);
}

void fdt_get_array_from_prop(fdtinfo_t* info, uint32_t propOffset, uintptr_t* array, size_t length){
    uint32_t offset = propOffset + 2;
    for(size_t i = 0; i < length; i++){
        array[i] = rtl_bswap32(info->cells[offset + i]);
    }
}

// doesn't work
// const char* fdt_get_string_from_prop(fdtinfo_t* info, uint32_t propOffset){
//     return fdt_get_string(info, rtl_bswap32(info->cells[propOffset + 2]));
// }
