#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED







#include <voldef.h>

// because the pagefreelist used in the pmm is
// 16 bytes large on most 64bit systems and 8 bytes large on
// most 32bit systems, make sure the page size is 16 bytes or greater
#define PAGE_SIZE 0x1000

typedef enum memorytype {
    MEM_TYPE_USED,
    MEM_TYPE_FREE,
    MEM_TYPE_MMIO
} memorytype_t;

typedef struct memoryentry {
    uintptr_t base;
    uintptr_t size;
    memorytype_t type;
} memoryentry_t;

typedef struct arena {
    uintptr_t base;
    uintptr_t size;
} arena_t;

typedef struct memorymap {
    memoryentry_t* entries;
    size_t amount;
    size_t sizeOfMemory;
} memorymap_t;

void mem_dbg_print_memmap();
bool mem_setup_pmm(memorymap_t* memMap);
void* mem_allocate_page();
bool mem_free_page(void* address);
size_t mem_return_used_pages();
size_t mem_return_total_pages();
#endif
