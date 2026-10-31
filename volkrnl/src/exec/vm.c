#include <exec/vm.h>
#include <memory/memory.h>
#include <rtl/mem.h>
#include <kernel/kernel.h>
#include <rtl/print.h>


// PoC, will need to do INSANE optimizations for this to even be viable
// TODO: optimize =D

static inline void add_page_to_memlist(vmmemlist_t* mem, vmmemlist_t** head, vmmemlist_t** tail){
    mem->next = NULL;
    if(*head == NULL){
        *head = mem;
    }
    if(*tail == NULL){
        *tail = mem;
    } else {
        mem->prev = *tail;
        (*tail)->next = mem;
        *tail = mem;
    }
}

static inline void del_page_from_memlist(vmmemlist_t* mem, vmmemlist_t** head, vmmemlist_t** tail){
    if(mem == *head){
        *head = mem->next;
    }
    if(mem == *tail){
        *tail = mem->prev;
    }
    if(mem->prev != NULL){
        mem->prev->next = mem->next;
    }
    if(mem->next != NULL){
        mem->next->prev = mem->prev;
    }
}

static inline vmmemlist_t* add_page_to_proc(vmproc_t* proc){
    vmmemlist_t* mem = mem_allocate_page();
    if(mem == NULL){
        // TODO: do something better then just panicking
        kern_panic("oom");
    }
    add_page_to_memlist(mem, &proc->memHead, &proc->memTail);
    proc->amountOfPages++;
    return mem;
}

static inline void del_page_from_proc(vmproc_t* proc, vmmemlist_t* mem){
    if(proc->amountOfPages == 0){
        DEBUG_WARNING("del_page_from_memlist was called when... theres no pages in this process\n");
    }
    del_page_from_memlist(mem, &proc->memHead, &proc->memTail);
    mem_free_page(mem);
    proc->amountOfPages--;
}

// these are stub functions and HAVE NOT BEEN TESTED!!!
bool exec_init_proc_from_raw(uint8_t* data, size_t dataSize, vmproc_t* proc){
    size_t upSize = PAGE_SIZE - sizeof(vmmemlist_t);
    proc->memHead = NULL;
    proc->memTail = NULL;
    for(size_t i = 0; i < dataSize; i+=upSize){
        vmmemlist_t* memList = add_page_to_proc(proc);
        memList->used = 0;
        void* mem = (void*)((uintptr_t)memList + sizeof(vmmemlist_t));
        size_t size = upSize;
        if(upSize > dataSize){
            size = dataSize;
        }
        DEBUG_INFO("copying data with offset %d to mem (size: %d)\n", (i), size);
        rtl_copy_mem((void*)((uintptr_t)data + i), mem, size);
    }
    return TRUE;
}

bool exec_cleanup_proc(vmproc_t* proc){
    vmmemlist_t* list = proc->memHead;
    while(list != NULL){
        vmmemlist_t* curList = list;
        list = list->next;
        del_page_from_proc(proc, curList);
    }
    return TRUE;
}
