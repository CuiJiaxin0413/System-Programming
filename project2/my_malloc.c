#include "my_malloc.h"



// for lock version_lock
MetaData * heap_start_lock = NULL;

pthread_mutex_t lock;

// thread local heap_start
__thread MetaData * heap_start_local = NULL;

MetaData * extend_heap(size_t size, int is_lock) {
    MetaData * new_block = NULL;
    if (is_lock) {
        new_block =  sbrk(size);
    } else {
        pthread_mutex_lock(&lock);
        new_block = sbrk(size);
        pthread_mutex_unlock(&lock);
    }

    return new_block;
}
 
// init a sentinel node for linked list
MetaData * init(int is_lock) {
    MetaData * heap_start = NULL;

    heap_start = extend_heap(META_DATA_SIZE, is_lock);

    heap_start->size = 0;
    heap_start->next = heap_start;
    heap_start->prev = heap_start;

    return heap_start;
}

// using best fit policy to get a free block that can be reused
MetaData * get_next_block_bf(size_t size, MetaData * heap_start) {
    //printf("in next: %p\n", heap_start);
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

// remove a block from the free list
void removeFromFreeList(MetaData * p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
}

// add a block to the free list
// according to the block address in ascending order
void addToFreeList(MetaData * p, MetaData * heap_start) {
    //printf("line72:%p\n", heap_start);
    MetaData * curr = heap_start->next;
    while (curr < p && curr != heap_start) {
        curr = curr->next;
    }
    p->next = curr;
    p->prev = curr->prev;
    curr->prev->next = p;
    curr->prev = p;
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

// merge block p with p->prev to get a bigger free space
void mergePrev(MetaData * p) {
    size_t prev_block_size = META_DATA_SIZE + p->prev->size;
    MetaData * prev_block = p->prev;

    // do not merge with the sentinal
    if (prev_block == heap_start_lock) return;

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

    // do not merge with the sentinal
    if (next_block == heap_start_lock) return;

    if ((char *) p + curr_block_size == (char *) next_block) {
        size_t next_block_size = META_DATA_SIZE + p->next->size;
        p->size += next_block_size;
        removeFromFreeList(next_block);
    }
}

void *bf_malloc(size_t size, int is_lock) {
    if (is_lock && heap_start_lock == NULL) {
        heap_start_lock = init(is_lock);
    } else if (!is_lock && heap_start_local == NULL) {
        heap_start_local = init(is_lock);
    }

    MetaData * heap_start = NULL;
    if (is_lock) {
        heap_start = heap_start_lock;
    } else {
        heap_start = heap_start_local;
    } 

    MetaData * next = get_next_block_bf(size, heap_start); 

    // cannot find a free block that can be reused, 
    // extend the heap to create a new block
    if (next == NULL) {
        size_t block_size = META_DATA_SIZE + size;
        
        next = extend_heap(block_size, is_lock);

        // check sbrk
        if (next == (void *) - 1) {
            return NULL;
        }
        
        next->size = size;
        next->prev = NULL;
        next->next = NULL;
    
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

void bf_free(void *ptr, MetaData * heap_start, int is_lock) {
    if (heap_start == NULL) {
        heap_start = init(is_lock);
    }
    MetaData * p = (MetaData *)((char*) ptr - META_DATA_SIZE);
    addToFreeList(p, heap_start);
    mergeNext(p);
    mergePrev(p);
}

//Thread Safe malloc/free: locking version
void *ts_malloc_lock(size_t size) {
    pthread_mutex_lock(&lock);
	void * p = bf_malloc(size, 1);
	pthread_mutex_unlock(&lock);
  	return p;
}

void ts_free_lock(void *ptr) {
    pthread_mutex_lock(&lock);
    bf_free(ptr, heap_start_lock, 1);
    pthread_mutex_unlock(&lock);
}

//Thread Safe malloc/free: non-locking version
void *ts_malloc_nolock(size_t size) {
    return bf_malloc(size, 0);
}

void ts_free_nolock(void *ptr) {
    bf_free(ptr, heap_start_local, 0);
}
