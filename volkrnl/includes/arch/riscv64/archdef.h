#ifndef ARCHDEF_H_INCLUDED
#define ARCHDEF_H_INCLUDED




#include <types.h>


#define MTIMECMP_OFFSET 0x4000
#define MTIME_OFFSET 0xBFF8



struct registerstate {
    uint64_t ra;
    uint64_t sp;
    uint64_t gp;
    uint64_t tp;
    uint64_t t0;
    uint64_t t1;
    uint64_t t2;
    uint64_t t3;
    uint64_t t4;
    uint64_t t5;
    uint64_t t6;
    uint64_t fp;
    uint64_t s1;
    uint64_t a0;
    uint64_t a1;
    uint64_t a2;
    uint64_t a3;
    uint64_t a4;
    uint64_t a5;
    uint64_t a6;
    uint64_t a7;
    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;
    uint64_t s0;
};



// setup stuff (yeah bc it has setup in its name lmao im so funny snsmksl)
void arch_setup_exceptions();

// register shenanigans
uint64_t arch_get_mhartid();
uint64_t arch_get_mie();
uint64_t arch_get_mepc();
uint64_t arch_get_mcause();
uint64_t arch_get_mtvec();
uint64_t arch_get_stvec();
uint64_t arch_get_mscratch();
uint64_t arch_get_mstatus();

void arch_set_mie(uint64_t value);
void arch_set_mcause(uint64_t value);
void arch_set_mtvec(uint64_t value);
void arch_set_stvec(uint64_t value);
void arch_set_mscratch(uint64_t value);
void arch_set_mstatus(uint64_t value);


// sync traps
void ins_addr_misaligned();
void ins_access_fault();
void illegal_instruction();
void load_address_misaligned();
void load_access_fault();
void store_address_misaligned();
void store_access_fault();
void ins_page_fault();
void load_page_fault();
void store_page_fault();

#endif
