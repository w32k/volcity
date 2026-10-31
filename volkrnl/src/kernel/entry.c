#include <rtl/print.h>
#include <memory/memory.h>
#include <kernel/kernel.h>
#include <arch/arch.h>
#include <arch/riscv64/archdef.h>

#include <fs/ustar/ustar.h>
#include <rtl/mem.h>
#include <platform/platform.h>
#include <exec/vm.h>

void kern_entry(memorymap_t* memMap, initrdinfo_t* rdInfo){
    rtl_printf("\n\nVolcity Version INDEV (%s at %s)\n\n", __DATE__, __TIME__);
    DEBUG_INFO("setting up memory manager\n");
    bool result = mem_setup_pmm(memMap);
    if(result == FALSE){
        kern_panic("couldn't setup memory manager");
    }
    mem_dbg_print_memmap();


    //size_t amount = 0;
    // for(;;){
    //     void* mem = mem_allocate_page();
    //     if(mem == NULL) break;
    //     rtl_zero_mem(mem, amount);
    //     amount++;
    // }
    //DEBUG_INFO("amount of pages found: %ld\n", amount);
    uint8_t data[8128] = {0};
    rtl_set_mem(data, 'h', 8128);
    vmproc_t proc = {0};
    exec_init_proc_from_raw(data, 8128, &proc);
    vmmemlist_t* list = proc.memHead;
    while(list != NULL){
        char* data = (char*)((uintptr_t)list + sizeof(vmmemlist_t));
        rtl_print("--------------------------------------------------------------------------------------------\n");
        for(size_t i = 0; i < PAGE_SIZE - sizeof(vmmemlist_t); i++){
            rtl_printf("%c", data[i]);
            if(i % 64 == 0) rtl_print("\n");
        }
        rtl_print("\n");

        list = list->next;
    }
    exec_cleanup_proc(&proc);
    DEBUG_INFO("%d\\%d pages used\n\n", mem_return_used_pages(), mem_return_total_pages());
    ustar_list_all_from_memory(rdInfo->base);

    while(1){continue;}
}
