#include <platform/platform.h>
#include <arch/riscv64/archdef.h>
#include <kernel/kernel.h>





void ins_addr_misaligned(){
    kern_panic("Instruction address misaligned");
}

void ins_access_fault(){
    kern_panic("Instruction access fault");
}

void illegal_instruction(){
    kern_panic("Illegal instruction");
}

void load_address_misaligned(){
    kern_panic("Load address misaligned");
}

void load_access_fault(){
    kern_panic("Load access fault");
}


void store_address_misaligned(){
    kern_panic("Store address misaligned");
}

void store_access_fault(){
    kern_panic("Store access fault");
}

void ins_page_fault(){
    kern_panic("Instruction page fault");
}

void load_page_fault(){
    kern_panic("Load page fault");
}

void store_page_fault(){
    kern_panic("Store page fault");
}
