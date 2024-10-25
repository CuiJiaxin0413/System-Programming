## ECE 650 Project #1: Malloc Library

For this project, I implemented my own version of two memory allocation functions from the C standard library: 

- `void *malloc(size_t size)`; 
- `void free(void *ptr)`;
  
For each of the functions, I implement 2 versions of malloc and free using different allocation policies, which are First Fit (FF) and Best Fit (BF) policy. First-fit policy allocates at the first free region with enough space, while best-fit policy allocates at the free region with smallest but enough free space.

To allocate more space from the heap, `sbrk()` should be used. `sbrk()` increments the program's data space by increment bytes.  Calling `sbrk()` with an increment of 0 can be used to find the current location of the program break. [ref: man page].

For more details, please refer to my report: https://docs.google.com/document/d/1FO9xDJ44G8iurbofD-m2jn2aD1m6IgOVfgYVqc4ncl4/edit?tab=t.0
