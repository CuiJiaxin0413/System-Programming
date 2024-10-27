# **ECE 651 Project #2:** 

**Thread-Safe Malloc Report**

**Jacey Cui (jc959)** 

**jiaxin.cui@duke.edu**

# **1. Overview**

For this project, I implemented my own version of thread-safe malloc library, using best-fit as malloc policy. I implemented 2 thread-safe versions, one use lock while the other does not. The overall design and data structures are same as what I did in project #1, so I only explain the additional parts that allow my program to run concurrently.

# **2. Implementation Description**

- ## **Lock Version**

For the lock version, it uses only one free list, so every addition/deletion operation to the free list should be the critical section. Since all the functions involve these operations, I added lock in my own version malloc and free to synchronize all the threads to avoid racing conditions.

The synchronization strategy is to use `pthread_mutex_t lock`. Whenever a thread is going to do a malloc or free call, use `pthread_mutex_lock(&lock)` to make sure only one thread can obtain the lock, thus only one thread can do operations to the global free list. After malloc or free call is returned, use `pthread_mutex_unlock(&lock)` to release the lock, so that the next thread that obtains the lock can do malloc or free.

```c	
//Thread Safe malloc/free: locking version
void * ts_malloc_lock(size_t size) {
    pthread_mutex_lock(&lock);
    void * p = bf_malloc(size, 1);
    pthread_mutex_unlock(&lock);
    return p;
}

```

```c
void ts_free_lock(void *ptr) {
    pthread_mutex_lock(&lock);
    bf_free(ptr, heap_start_lock, 1);
    pthread_mutex_unlock(&lock);
}
```



- ## **Non-lock version**

For the non-lock version, I use thread local to represent the free list head, so that each thread will have its own free list. In this way, you do not need to worry about the racing conditions in the free list. But in this case, the heap memory is still shared by multiple threads, so the shared resource in the non-lock version is actually the memory in heap. As a result, we need to synchronize when it is going to extend the heap, which is when it calls `sbrk()`, and that is the critical section.

The synchronization strategy is to use mutex lock before and after call `sbrk()`. Unlike the lock version, we do not synchronize the free list, because each thread has its own free list in this case.

```c
pthread_mutex_lock(&lock); new_block = sbrk(size); pthread_mutex_unlock(&lock);
```



```c
//Thread Safe malloc/free: non-locking version void *ts_malloc_nolock(size_t size) {   
	return bf_malloc(size, 0); 
}  
void ts_free_nolock(void *ptr) {   
	bf_free(ptr, heap_start_local, 0); 
}
```





# **3. Performance Result Presentation**

- ## **Lock Version**

**![img](https://lh7-rt.googleusercontent.com/docsz/AD_4nXexputTdbfP-8h5LpXVKziNYaC3NXiWuVr73Aq-Jln5ek7tIMP4uyrURkWR0hmi_Npr4NjbJIclanRODd9dFUVAkv-6bYH_-ptanU85xr9tLgzPCVRvyHiatk6DpQJJUpVlWhOO4EQBVTDKBZeb2NykjQli?key=pf7YsxLAN2PL7tTKiVIJMA)**

- ## **Non-lock version**

**![img](https://lh7-rt.googleusercontent.com/docsz/AD_4nXev3BQ6YaQ01ykIDd_ErKilJVJ_kQA14_VfbXGyjGpp6jFlCUf1pnSiVtOznQLx9dKm9m7gsn6WqhuW0UUwltg4KIxo-4fxqa7Gm7mDjcd9otbMFC7PKQNUvQu2P9_ZB22_A59dAdcma88jTSi0a0oosm-s?key=pf7YsxLAN2PL7tTKiVIJMA)**

# **4. Results Analysis**

- ## **Trade-Offs**

It is shown that the lock version takes more time than the non-lock version, it is because the lock version implementation only allows one thread to enter each time, while the non-lock version uses thread local, multiple threads can do malloc and free at the same time. Although the non-lock version also uses mutex lock when it tries to extend the heap, the critical section is much smaller than that in the lock version. So generally speaking, the lock version should take more time than the non-lock version.

On the other hand, since non-lock versions require a thread local free list for each thread, it takes more space to record the free list than the lock version, so there is less available heap memory for it to use than the lock version. And also, since the lock version shares one free list, it has a greater possibility that it can find a reusable block in the free list, so that it does not need to allocate new space in the heap, while the non-lock version might not find a reusable block within the thread.

# **5. Reflections**

- ## **Pitfalls**

Some mistakes I made during the time I did this assignment:

1. Found a bug in my implementation for project #1, I did not figure it out until my program needed to run concurrently. In merge part: a block should not be merged with the sentinel node.
2. What if a thread free memory space that was allocated by other threads?This typically is not a problem. It really depends on which thread is running when you add a block to the free list, and it is a non-deterministic behavior. It could be the thread adding the block to its thread local free list, and do malloc the next time. In this case, the block can be reused. But just make sure that the free list should always have a sentinel node, and that is enough for this implementation.