#include <exec/vm.h>
#include <memory/memory.h>
#include <rtl/mem.h>
#include <kernel/kernel.h>
#include <rtl/print.h>


// Proof of Concept per vm process memory manager
// this has a MASSIVE flaw, and this is with how we free pages
// the way we do it rn is we manage it so the VM deals with an illusion of a flat memory table, even though its just linked lists
// where each node corresponds with a page, with node->next corresponding with page + 1 (if node is 0x1000 - 0x1fff, node->next is page 0x2000 - 0x2fff)
// to deal with iteration speed, we have node->nextThousand and node->nextMillion, kind of hacky but it works
// and we free pages not being used, say 0x21000 - 0x22fff wasn't mapped, and if its needed it can be mapped again
// the issue is, if we free a middle page, we remove it from the list to not waste memory as our physical memory manager is VERY primative right now
// but say we free the page 0x1000, the list will go out of order by a page, making page 0x2000 go down to 0x1000 etc, which can break a ton of stuff
// theres many solutions, but all have draw backs, we can create a side pool allocater of way smaller memlist structs to fill in the holes
// but this'll waste precious memory we don't have sadly, we can also just not do the approach above and instead tag each page
// but this'll make lookup extremely slow. We can also just not free middle pages, and only free pages that are at the tail
// this is bad because itll cause pages that can be used by other processes just sit in processes where it isn't used at all
// the final solution is we scrap this idea, make an ACTUAL good physical memory manager which can allocate multiple pages, and remake this so its
// way simpler and ACTUALLY works, this is good but this'll also make the physical memory manager a bit slower (the simple pmm we have rn is pretty fast)
// and will also waste my very tiny time just rewriting stuff so it works



typedef enum pagelevel {
    PAGELEVEL_ONE,
    PAGELEVEL_THOUSAND,
    PAGELEVEL_MILLION
} pagelevel_t;

static inline void add_page_to_memlist(vmmemlist_t* mem, vmmemlist_t** head, vmmemlist_t** tail, pagelevel_t pagelevel){
    mem->next = NULL;
    switch(pagelevel){
        case PAGELEVEL_ONE:{
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
            break;
        }
        case PAGELEVEL_THOUSAND:{
            if(*head == NULL){
                *head = mem;
            }
            if(*tail == NULL){
                *tail = mem;
            } else {
                mem->prevThousand = *tail;
                (*tail)->nextThousand = mem;
                *tail = mem;
            }
            break;
        }
        case PAGELEVEL_MILLION:{
            if(*head == NULL){
                *head = mem;
            }
            if(*tail == NULL){
                *tail = mem;
            } else {
                mem->prevMillion = *tail;
                (*tail)->nextMillion = mem;
                *tail = mem;
            }
            break;
        }
        default:
            kern_panic("invalid pagelevel\n");
            break;
    }

}

static inline void del_page_from_memlist(vmmemlist_t* mem, vmmemlist_t** head, vmmemlist_t** tail, pagelevel_t pagelevel){
    switch(pagelevel){
        case PAGELEVEL_ONE:{
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
            break;
        }
        case PAGELEVEL_THOUSAND:{
            if(mem == *head){
                *head = mem->nextThousand;
            }
            if(mem == *tail){
                *tail = mem->prevThousand;
            }
            if(mem->prevThousand != NULL){
                mem->prevThousand->nextThousand = mem->nextThousand;
            }
            if(mem->nextThousand != NULL){
                mem->nextThousand->prevThousand = mem->prevThousand;
            }
            break;
        }
        case PAGELEVEL_MILLION:{
            if(mem == *head){
                *head = mem->nextMillion;
            }
            if(mem == *tail){
                *tail = mem->prevMillion;
            }
            if(mem->prevMillion != NULL){
                mem->prevMillion->nextMillion = mem->nextMillion;
            }
            if(mem->nextMillion != NULL){
                mem->nextMillion->prevMillion = mem->prevMillion;
            }
            break;
        }
    }
}


static inline vmmemlist_t* add_page_to_proc(vmproc_t* proc){
    vmmemlist_t* mem = mem_allocate_page();
    if(mem == NULL){
        // TODO: do something better then just panicking
        kern_panic("oom");
    }
    add_page_to_memlist(mem, &proc->memHead, &proc->memTail, PAGELEVEL_ONE);
    proc->amountOfPages++;

    // NOT TESTED
    if(proc->amountOfPages % 1000 == 0){
        add_page_to_memlist(mem, &proc->memThousandHead, &proc->memThousandTail, PAGELEVEL_THOUSAND);
    }
    if(proc->amountOfPages % 1000000 == 0){
        add_page_to_memlist(mem, &proc->memMillionHead, &proc->memMillionTail, PAGELEVEL_MILLION);
    }
    mem->pageIndex = proc->amountOfPages;
    return mem;
}

static inline void del_page_from_proc(vmproc_t* proc, vmmemlist_t* mem){
    if(proc->amountOfPages == 0){
        DEBUG_WARNING("del_page_from_memlist was called when... theres no pages in this process\n");
    }
    del_page_from_memlist(mem, &proc->memHead, &proc->memTail, PAGELEVEL_ONE);

    // NOT TESTED
    if(mem->pageIndex % 1000 == 0){
        del_page_from_memlist(mem, &proc->memThousandHead, &proc->memThousandTail, PAGELEVEL_THOUSAND);
    }
    if(mem->pageIndex % 1000000 == 0){
        del_page_from_memlist(mem, &proc->memMillionHead, &proc->memMillionTail, PAGELEVEL_MILLION);
    }
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
