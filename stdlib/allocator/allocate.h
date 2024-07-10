#ifndef __ALOCATE_H
#define __ALOCATE_H

typedef char byte_t;
typedef unsigned long size_t;

extern void* brk(void*);

extern void* endheap;

extern void init_alloc();

extern void deinit_alloc();

extern void* alloc(size_t size);

extern void free_alloc(void* alocated_ptr);

#endif