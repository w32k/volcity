#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED







#include <types.h>

// because the pagefreelist used in the pmm is
// 16 bytes large on most 64bit systems and 8 bytes large on
// most 32bit systems, make sure the page size is 16 bytes or greater
#define PAGE_SIZE 0x1000

typedef enum _MemoryType {
    MEM_TYPE_USED,
    MEM_TYPE_FREE,
    MEM_TYPE_MMIO
} MemoryType;

typedef struct _MemoryEntry {
    uintptr_t base;
    uintptr_t size;
    MemoryType type;
} MemoryEntry;

typedef struct _Arena {
    uintptr_t base;
    uintptr_t size;
} Arena;

typedef struct _MemoryMap {
    MemoryEntry* entries;
    size_t amount;
    size_t sizeOfMemory;
} MemoryMap;

void mem_dbg_print_memmap();
bool mem_setup_pmm(MemoryMap* memMap);
void* mem_allocate_page();
bool mem_free_page(void* address);

#endif
