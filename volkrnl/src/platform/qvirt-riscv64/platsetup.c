
#include <platform/platform.h>
#include <arch/arch.h>
#include <arch/riscv64/archdef.h>

#include <memory/memory.h>
#include <device/fdt/fdt.h>
#include <kernel/kernel.h>
#include <rtl/print.h>


static memoryentry_t memEntries[256];
static memorymap_t memMap = {0};
static initrdinfo_t initRdInfo = {0};


extern uintptr_t k_end;

static inline bool retrieve_memory_info(fdtinfo_t* info, uintptr_t* base, size_t* size){
    uint32_t node = 0;
    bool result = fdt_find_node(info, "memory@80000000", &node);
    if(result == FALSE){
        DEBUG_FAIL("unable to locate memory node\n");
        return FALSE;
    }
    uint32_t propOffset = 0;
    fdtprop_t* prop = fdt_get_prop_ex(info, node, "reg", &propOffset);
    if(prop == NULL){
        DEBUG_FAIL("unable to locate reg prop in memory node\n");
        return FALSE;
    }

    uintptr_t array[4] = {0};
    fdt_get_array_from_prop(info, propOffset, array, 4);

    *base = ((uint64_t)array[0] << 32 | (uint32_t)array[1]);
    *size = ((uint64_t)array[2] << 32 | (uint32_t)array[3]);
    return TRUE;
}

static inline bool retrieve_initrd_info(fdtinfo_t* info, uintptr_t* base, size_t* size){
    uint32_t node = 0;
    bool result = fdt_find_node(info, "chosen", &node);
    if(result == FALSE){
        DEBUG_FAIL("unable to locate chosen node\n");
        return FALSE;
    }
    uint32_t basePropOffset = 0;
    fdtprop_t* baseProp = fdt_get_prop_ex(info, node, "linux,initrd-start", &basePropOffset);
    if(baseProp == NULL){
        DEBUG_FAIL("unable to locate initrd-start prop in memory node\n");
        return FALSE;
    }

    uintptr_t baseArray[2] = {0};
    fdt_get_array_from_prop(info, basePropOffset, baseArray, 2);
    uintptr_t baseLoc = ((uint64_t)baseArray[0] << 32 | (uint32_t)baseArray[1]);
    *base = baseLoc;

    uint32_t endPropOffset = 0;
    fdtprop_t* endProp = fdt_get_prop_ex(info, node, "linux,initrd-end", &endPropOffset);
    if(endProp == NULL){
        DEBUG_FAIL("unable to locate initrd-end prop in memory node\n");
        return FALSE;
    }

    uintptr_t endArray[2] = {0};
    fdt_get_array_from_prop(info, endPropOffset, endArray, 2);
    uintptr_t endLoc = ((uint64_t)endArray[0] << 32 | (uint32_t)endArray[1]);
    *size = endLoc - baseLoc;
    return TRUE;
}



void plat_setup(uintptr_t dTreeBase, uintptr_t kernelEndpoint){
    plat_uart_setup();
    arch_setup();
    if(arch_get_stvec() == 0x0 && arch_get_mtvec() == 0x0){
        kern_panic("mtvec and stvec are broken");
    }
    rtl_printf("mtvec: 0x%lx\n", arch_get_mtvec());

    fdtinfo_t info = {0};
    bool result = fdt_init(&info, dTreeBase);
    if(result == FALSE){
        DEBUG_FAIL("invalid fdt! magic returned is 0x%x\n", info.header.magic);
        kern_panic("invalid fdt");
    }

    uintptr_t memBase = 0;
    size_t memSize = 0;
    result = retrieve_memory_info(&info, &memBase, &memSize);
    if(result == FALSE){
        kern_panic("couldn't receive memory info from device tree");
    }

    uintptr_t initRdBase = 0;
    size_t initRdSize = 0;
    result = retrieve_initrd_info(&info, &initRdBase, &initRdSize);
    if(result == FALSE){
        kern_panic("couldn't receive initrd info from device tree");
    }

    // rtl_printf("memory base is 0x%lx, memory size is %ld\n", memBase, memSize);
    // rtl_printf("initrd base is 0x%lx, initrd size is %ld\n", initRdBase, initRdSize);
    // rtl_printf("dTreeLocation is 0x%lx\n", dTreeBase);
    initRdInfo.base = initRdBase;
    initRdInfo.size = initRdSize;

    memMap.sizeOfMemory = memSize;

    // very very messy code, bc im stupid =(
    // like this is genuinely horrible BUT
    // it works and im too lazy to change it
    // for UEFI based systems we would probably get a better
    // memmap from EFI to our (non-existent, soon (tm)) bootloader
    // and then pass it to the kernel later down the road
    // this isn't UEFI however so uh enjoy this mess!
    memEntries[0].base = 0x0;
    memEntries[0].size = memBase;
    memEntries[0].type = MEM_TYPE_MMIO;

    memEntries[1].base = memBase;
    memEntries[1].size = kernelEndpoint - memBase;
    memEntries[1].type = MEM_TYPE_USED;
    //rtl_printf("binary size is %ld, kernelEndpoint: 0x%x\n", memEntries[0].size, kernelEndpoint);
    memEntries[2].base = kernelEndpoint;
    memEntries[2].size = initRdBase - kernelEndpoint;
    memEntries[2].type = MEM_TYPE_FREE;

    memEntries[3].base = initRdBase;
    memEntries[3].size = initRdSize;
    memEntries[3].type = MEM_TYPE_USED;

    memEntries[4].base = initRdBase + initRdSize;
    memEntries[4].size = dTreeBase - memEntries[3].base;
    memEntries[4].type = MEM_TYPE_FREE;

    memEntries[5].base = dTreeBase;
    memEntries[5].size = info.header.totalSize;
    memEntries[5].type = MEM_TYPE_USED;

    memEntries[6].base = dTreeBase + info.header.totalSize;
    // stupid hack, TODO: find why it actually breaks
    memEntries[6].size = memSize - (memEntries[6].base - memBase);
    memEntries[6].type = MEM_TYPE_FREE;
    memMap.entries = memEntries;
    memMap.amount = 7;


    //*(uint64_t*)(0x90290292) = 'h';

    kern_entry(&memMap, &initRdInfo);
    kern_panic("kern_entry exited!");
}
