#include <rtl/print.h>
#include <memory/memory.h>
#include <kernel/kernel.h>
#include <arch/arch.h>
#include <arch/riscv64/archdef.h>

#include <fs/ustar/ustar.h>
#include <rtl/mem.h>
#include <platform/platform.h>

void kern_entry(MemoryMap* memMap, InitRdInfo* rdInfo){
    rtl_printf("\n\nVolcity Version INDEV (%s at %s)\n\n", __DATE__, __TIME__);
    DEBUG_INFO("setting up memory manager\n");
    bool result = mem_setup_pmm(memMap);
    if(result == FALSE){
        kern_panic("couldn't setup memory manager");
    }
    mem_dbg_print_memmap();


    size_t amount = 0;
    for(;;){
        void* mem = mem_allocate_page();
        if(mem == NULL) break;
        rtl_zero_mem(mem, amount);
        amount++;
    }
    DEBUG_INFO("amount of pages found: %ld\n", amount);
    //this will cause a crash
    // uint8_t p = *(uint8_t*)(0x2992829829189);
    // p++;
    // (void)p;
    ustar_list_all_from_memory(rdInfo->base);

    while(1){continue;}
}
