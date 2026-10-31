#include <arch/arch.h>
#include <arch/riscv64/archdef.h>


extern void exception_handler();

void arch_setup(){
    arch_set_mtvec((uintptr_t)exception_handler & ~0b11);
}


void arch_halt(){
    __asm__("wfi");
    while(1){continue;}
}
