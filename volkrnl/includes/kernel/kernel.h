#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED







#include <types.h>
#include <memory/memory.h>

typedef struct initrdinfo {
    uintptr_t base;
    size_t size;
} initrdinfo_t;

void kern_entry(memorymap_t* memMap, initrdinfo_t* info);
[[noreturn]] void kern_panic(const char* reason);
void kern_parse_tar(uintptr_t* address);











#endif
