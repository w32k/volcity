#ifndef ARCH_H_INCLUDED
#define ARCH_H_INCLUDED







#include <voldef.h>


typedef struct registerstate registerstate_t;


void arch_setup();
void arch_dump_and_print_registers();
void arch_dump_registers(registerstate_t* state);
void arch_print_registers(registerstate_t* state);
void arch_load_registers(registerstate_t* state);
void arch_halt();
uintptr_t arch_get_current_core();






#endif
