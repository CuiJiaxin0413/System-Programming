#ifndef __MY_MALLOC_H__
#define __MY_MALLOC_H__

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define META_DATA_SIZE sizeof(MetaData)

typedef struct MetaData {
    size_t size;
    struct MetaData * next;
    struct MetaData * prev;
} MetaData;

//Thread Safe malloc/free: locking version
void *ts_malloc_lock(size_t size);
void ts_free_lock(void *ptr);
//Thread Safe malloc/free: non-locking version
void *ts_malloc_nolock(size_t size);
void ts_free_nolock(void *ptr);

#endif