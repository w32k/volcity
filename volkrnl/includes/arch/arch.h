#ifndef ARCH_H_INCLUDED
#define ARCH_H_INCLUDED







#include <types.h>


typedef struct _RegisterState RegisterState;


void arch_setup();
void arch_dump_and_print_registers();
void arch_dump_registers(RegisterState* state);
void arch_print_registers(RegisterState* state);
void arch_load_registers(RegisterState* state);
void arch_halt();
uintptr_t arch_get_current_core();






#endif
