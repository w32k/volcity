#include <kernel/kernel.h>
#include <rtl/print.h>
#include <arch/riscv64/archdef.h>


typedef void(*Callback)();

void unimplemented_callback(){DEBUG_INFO("unimplemented\n");};


Callback asyncCallbacks[] = {
    [0] = unimplemented_callback,
    [1] = unimplemented_callback,
    [2] = unimplemented_callback,
    [3] = unimplemented_callback,
    [4] = unimplemented_callback,
    [5] = unimplemented_callback,
    [6] = unimplemented_callback,
    [7] = unimplemented_callback,
    [8] = unimplemented_callback,
    [9] = unimplemented_callback,
    [10] = unimplemented_callback,
    [11] = unimplemented_callback,
    [12] = unimplemented_callback,
    [13] = unimplemented_callback,
    [14] = unimplemented_callback,
    [15] = unimplemented_callback,
    [16] = unimplemented_callback,
};


Callback syncCallbacks[] = {
    [0] = ins_addr_misaligned,
    [1] = ins_access_fault,
    [2] = illegal_instruction,
    [3] = unimplemented_callback,
    [4] = load_address_misaligned,
    [5] = load_access_fault,
    [6] = store_address_misaligned,
    [7] = store_access_fault,
    [8] = unimplemented_callback,
    [9] = unimplemented_callback,
    [10] = unimplemented_callback,
    [11] = unimplemented_callback,
    [12] = ins_page_fault,
    [13] = load_page_fault,
    [14] = unimplemented_callback,
    [15] = store_page_fault,
    [16] = unimplemented_callback,
};



// this is a mess
void exception_parser() {
    uint64_t mepc = arch_get_mepc();
    uint64_t mcause = arch_get_mcause();
    rtl_print_hex("mcause: ", mcause, TRUE);
    rtl_print_hex("mepc: ", mepc, TRUE);

    if(mcause & (1 << 31)){
        mcause &= ~(1 << 31);
        rtl_print_dec("parsed mcause: ", mcause, TRUE);
        uint32_t asyncArrSize = sizeof(asyncCallbacks) / sizeof(asyncCallbacks[0]);
        if(mcause > asyncArrSize){
            kern_panic("invalid mcause returned at exception");
        }
        asyncCallbacks[mcause]();
    } else {
        uint32_t syncArrSize = sizeof(syncCallbacks) / sizeof(syncCallbacks[0]);
        rtl_print_dec("size: ", syncArrSize, TRUE);
        if(mcause > syncArrSize){
           // rtl_print("hm\n");
            kern_panic("invalid mcause returned at exception (bit not set)");
        } else syncCallbacks[mcause]();
    }
}
