#include <memory/memory.h>
#include <rtl/print.h>
#include <rtl/mem.h>

struct pagefreelist {
    struct pagefreelist* prev;
    struct pagefreelist* next;
};

static memorymap_t* memMap = NULL;
static struct pagefreelist* head = NULL;
static struct pagefreelist* tail = NULL;
static size_t usedPages = 0;
static size_t totalPages = 0;


const char* memMapTypeToText[] = {
    [MEM_TYPE_USED] = "used",
    [MEM_TYPE_FREE] = "free",
    [MEM_TYPE_MMIO] = "mmio",
};


// TODO: add spinlocks to make it SMP-safe
// it isn't SMP safe rn =(

static inline void add_page_to_freelist(uintptr_t base){
    struct pagefreelist* page = (struct pagefreelist*)base;
    if(head == NULL){
        head = page;
    }
    if(tail == NULL){
        tail = page;
    } else {
        page->prev = tail;
        tail->next = page;
        tail = page;
    }
}

static inline void* get_page_from_freelist(){
    if(tail == NULL){
        return NULL;
    }
    struct pagefreelist* page = tail;
    tail = tail->prev;
    if(tail != NULL){
        tail->next = NULL;
    }
    if(head == page){
        head = NULL;
    }
    return (void*)page;
}

void mem_dbg_print_memmap(){
    const char* style = "--------------------------------------------------------------------------------\n";
    size_t usableMemory = 0;
    rtl_print("\n");
    rtl_print(style);
    rtl_print("dumping memory map\n");
    rtl_print(style);
    for(size_t i = 0; i < memMap->amount; i++){
        memoryentry_t entry = memMap->entries[i];
        const char* memMapType;
        if((size_t)entry.type >= ARR_SIZE(memMapTypeToText)){
            memMapType = "unknown";
        } else {
            memMapType = memMapTypeToText[entry.type];
        }
        rtl_printf("base: 0x%lx, end: 0x%lx, size: %ld, type: %s\n",
            entry.base, entry.base + entry.size, entry.size, memMapType);
        if(entry.type == MEM_TYPE_FREE){
            usableMemory += entry.size;
        }
    }
    rtl_print(style);
    rtl_printf("usable memory: %ld/%ld\n", usableMemory, memMap->sizeOfMemory);
    rtl_print(style);
}

bool mem_setup_pmm(memorymap_t* memoryMap){
    memMap = memoryMap;

    // this is really really slow with more and more amounts of memory
    // i need to do this in a smarter way
    for(size_t i = 0; i < memMap->amount; i++){
        memoryentry_t* entry = &memMap->entries[i];
        if(entry->type != MEM_TYPE_FREE) continue;
        for(size_t i = 0; i < entry->size; i+=PAGE_SIZE){
            // HACK: idk what to do with parts of memory that don't align with a page
            // its a waste of memory which is a shame, esp for more fragmented memory maps
            // but for now this will do!
            if(i + PAGE_SIZE > entry->size) break;
            add_page_to_freelist(entry->base + i);
            totalPages++;
        }
    }
    DEBUG_INFO("totalPages: %d\n", totalPages);
    return TRUE;
}

void* mem_allocate_page(){
    usedPages++;
    return get_page_from_freelist();
}

bool mem_free_page(void* address){
    if(usedPages == 0){
        DEBUG_WARNING("hey.... someones trying to free a page when theres none allocated!\n");
        return FALSE;
    }
    usedPages--;
    add_page_to_freelist((uintptr_t)address);
    return TRUE;
}

size_t mem_return_used_pages(){
    return usedPages;
}

size_t mem_return_total_pages(){
    return totalPages;
}
