#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED




#include <voldef.h>


typedef enum type {
    TYPE_BYTE, TYPE_SHORT, TYPE_INT, TYPE_LONG
} type_t;

typedef enum opcode {
    OP_BPUSH, OP_BPOP, OP_BADD, OP_BSUB, OP_BMUL, OP_BDIV, OP_BXOR, OP_BOR, OP_BAND, OP_BNEG, OP_BCMP, OP_BLD, OP_BSTR,
    OP_SPUSH, OP_SPOP, OP_SADD, OP_SSUB, OP_SMUL, OP_SDIV, OP_SXOR, OP_SOR, OP_SAND, OP_SNEG, OP_SCMP, OP_SLD, OP_SSTR,
    OP_IPUSH, OP_IPOP, OP_IADD, OP_ISUB, OP_IMUL, OP_IDIV, OP_IXOR, OP_IOR, OP_IAND, OP_INEG, OP_ICMP, OP_ILD, OP_ISTR,
    OP_LPUSH, OP_LPOP, OP_LADD, OP_LSUB, OP_LMUL, OP_LDIV, OP_LXOR, OP_LOR, OP_LAND, OP_LNEG, OP_LCMP, OP_LLD, OP_LSTR,
    OP_JMP, OP_JE, OP_JNE, OP_JL, OP_JG, OP_JLE, OP_JGE
} opcode_t;

typedef enum reg {
    REG_PC,
    REG_SL,
    REG_END
} reg_t;

typedef struct vmproc {
    uintptr_t reg[REG_END];
    uint8_t** memory;
    uint8_t* memoryMap;
} vmproc_t;





#endif
