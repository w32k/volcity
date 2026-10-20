#include <arch/arch.h>
#include <arch/riscv64/archdef.h>
#include <rtl/print.h>









void arch_print_registers(registerstate_t* state){
    rtl_print_hex("RA: ", state->ra, FALSE);
    rtl_print_hex("  SP: ", state->sp, TRUE);
    rtl_print_hex("GP: ", state->gp, FALSE);
    rtl_print_hex("  TP: ", state->tp, TRUE);
    rtl_print_hex("T0: ", state->t0, FALSE);
    rtl_print_hex("  T1: ", state->t1, TRUE);
    rtl_print_hex("T2: ", state->t2, FALSE);
    rtl_print_hex("  T3: ", state->t3, TRUE);
    rtl_print_hex("T4: ", state->t4, FALSE);
    rtl_print_hex("  T5: ", state->t5, TRUE);
    rtl_print_hex("T6: ", state->t6, FALSE);

    rtl_print_hex("  FP: ", state->fp, TRUE);
    rtl_print_hex("S1: ", state->s1, FALSE);
    rtl_print_hex("  A0: ", state->a0, TRUE);
    rtl_print_hex("A1: ", state->a1, FALSE);
    rtl_print_hex("  A2: ", state->a2, TRUE);
    rtl_print_hex("A3: ", state->a3, FALSE);
    rtl_print_hex("  A4: ", state->a4, TRUE);
    rtl_print_hex("A5: ", state->a5, FALSE);
    rtl_print_hex("  A6: ", state->a6, TRUE);
    rtl_print_hex("A7: ", state->a7, FALSE);

    rtl_print_hex("  S2: ", state->s2, TRUE);
    rtl_print_hex("S3: ", state->s3, FALSE);
    rtl_print_hex("  S4: ", state->s4, TRUE);
    rtl_print_hex("S5: ", state->s5, FALSE);
    rtl_print_hex("  S6: ", state->s6, TRUE);
    rtl_print_hex("S7: ", state->s7, FALSE);
    rtl_print_hex("  S8: ", state->s8, TRUE);
    rtl_print_hex("S9: ", state->s9, FALSE);
    rtl_print_hex("  S10: ", state->s10, TRUE);
    rtl_print_hex("S11: ", state->s11, FALSE);
    rtl_print_hex("  S0: ", state->s0, TRUE);
}
