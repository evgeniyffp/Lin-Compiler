#ifndef __ALOCATE_H
#define __ALOCATE_H

#include "../core.h"

extern void* brk(void*);

extern void* endheap;

extern void __malloc_init();
extern void __malloc_deinit();

extern void* __malloc(size_t size);
extern void __free(void* alocated_ptr);

#endif