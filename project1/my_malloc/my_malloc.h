#ifndef __MY_MALLOC_H__
#define __MY_MALLOC_H__

#include <stdlib.h>


#define META_DATA_SIZE sizeof(MetaData)

typedef struct MetaData {
    size_t size;
    struct MetaData * next;
    struct MetaData * prev;
} MetaData;

void init(void);
MetaData * get_next_block_ff(size_t size);
MetaData * get_next_block_bf(size_t size);
void split(MetaData * p, size_t size);
void removeFromFreeList(MetaData * p);
void *my_malloc(size_t size, int strategy);

//First Fit malloc/free
void *ff_malloc(size_t size);
void ff_free(void *ptr);
//Best Fit malloc/free
void *bf_malloc(size_t size);
void bf_free(void *ptr);

unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();


#endif