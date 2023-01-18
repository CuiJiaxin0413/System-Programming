#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "my_malloc.h"

// the low and high address of the heap
MetaData * heap_start = NULL;
MetaData * heap_top = NULL;

// init a sentinel node for linked list
void init(void) {
    heap_start = sbrk(META_DATA_SIZE);
    heap_top = sbrk(0);
    heap_start->size = 0;
    heap_start->next = heap_start;
    heap_start->prev = heap_start;
}

// strategy 1: ff, 0:bf
void *my_malloc(size_t size, int strategy) {
    if (heap_start == NULL) {
        init();
    }
    
    MetaData * next = strategy ? get_next_block_ff(size) : get_next_block_bf(size); 

    // cannot find a free block that can be reused, 
    // extend the heap to create a new block
    if (next == NULL) {
        size_t block_size = META_DATA_SIZE + size;
        next = (MetaData *) sbrk(block_size);
        heap_top = sbrk(0);

        next->size = size;
        next->prev = NULL;
        next->next = NULL;
        // check sbrk
        if (next == (void *) - 1) {
            return NULL;
        }
    } else {  
        // the block can be reused, just mark it as allocated and remove it from the free list
        // check if it can be splited: 
        // there is still enough space for MetaData after placing it with #size space
        if (next->size > size + META_DATA_SIZE) {
            split(next, size);
            removeFromFreeList(next);
        } else {
            // cannot be splited, just remove from free list
            removeFromFreeList(next);
        }
    }

    return (char *)next + META_DATA_SIZE;
}

// remove a block from the free list
void removeFromFreeList(MetaData * p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
}

// add a block to the free list
// according to the block address in ascending order
void addToFreeList(MetaData * p) {
    MetaData * curr = heap_start->next;
    while (curr < p && curr != heap_start) {
        curr = curr->next;
    }
    p->next = curr;
    p->prev = curr->prev;
    curr->prev->next = p;
    curr->prev = p;
}

// using first fit policy to get a free block that can be reused
MetaData * get_next_block_ff(size_t size) {
    MetaData * p = heap_start->next;
    for (; p != heap_start; p = p->next) {
        if (p->size >= size) {
            return p;
        }
    }
    // cannot find one
    return NULL;
}

// using best fit policy to get a free block that can be reused
MetaData * get_next_block_bf(size_t size) {
    MetaData * curr = heap_start->next;
    MetaData * best_fit = NULL;
    for (; curr != heap_start; curr = curr->next) {
        if (curr->size < size) {
            continue;
        }
        // to speed up: if curr->size == size
        // this should be the best, just return
        if (curr->size == size) {
            return curr;
        }
        if (best_fit == NULL || curr->size < best_fit->size) {
            best_fit = curr;
        }
    }
    return best_fit;
}

// merge block p with p->prev to get a bigger free space
void mergePrev(MetaData * p) {
    size_t prev_block_size = META_DATA_SIZE + p->prev->size;
    MetaData * prev_block = p->prev;

    if ((char *) p == (char *) prev_block + prev_block_size) {
        size_t curr_block_size = META_DATA_SIZE + p->size;
        prev_block->size += curr_block_size;
        removeFromFreeList(p);
    }
}

// merge p with p->next to get a bigger free space
void mergeNext(MetaData * p) {
    size_t curr_block_size = META_DATA_SIZE + p->size;
    MetaData * next_block = p->next;
    if ((char *) p + curr_block_size == (char *) next_block) {
        size_t next_block_size = META_DATA_SIZE + p->next->size;
        p->size += next_block_size;
        removeFromFreeList(next_block);
    }
}

// split a big block to be two blocks
// the size of the first block = size (request size)
// the size of the second block = the remaing part of the original big block
void split(MetaData * p, size_t size) {
    // total block p size = META_DATA_SIZE + size
    MetaData * new_block = (MetaData *)((char *) p + META_DATA_SIZE + size);
    new_block->size = p->size - size - META_DATA_SIZE;
    new_block->next = p->next;
    new_block->prev = p;

    p->size = size;
    p->next->prev = new_block;
    p->next = new_block;
}

//First Fit malloc/free
void *ff_malloc(size_t size) {
    return my_malloc(size, 1);

}

void ff_free(void *ptr) {
    MetaData * p = (MetaData *)((char*) ptr - META_DATA_SIZE);
    addToFreeList(p);
    mergeNext(p);
    mergePrev(p);
}


//Best Fit malloc/free
void *bf_malloc(size_t size) {
    return my_malloc(size, 0);

}

void bf_free(void *ptr) {
    ff_free(ptr);
}

unsigned long get_data_segment_size() {
    return (char *)heap_top - (char *)heap_start;
}

unsigned long get_data_segment_free_space_size() {
    size_t total_free_list_size = 0;
    MetaData * curr = heap_start->next;
    while (curr != heap_start) {
        total_free_list_size += curr->size + META_DATA_SIZE;
        curr = curr->next;
    }

    return total_free_list_size + META_DATA_SIZE;  // plus sentinel

}