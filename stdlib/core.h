#ifndef _STDLIB_CORE_H
#define _STDLIB_CORE_H

#ifndef bool
    #define bool int
#endif
#ifndef true
    #define true 1
#endif
#ifndef false
    #define false 0
#endif
#ifndef NULL
    #define NULL 0
#endif

typedef unsigned long size_t;
typedef char byte_t;

extern void __printf(char*, ...);
extern void __exit(int);
extern void* __brk(void*);
extern size_t __strlen(const char*);

#define assert(x, y) { if (x) { __printf(y); __exit(1); } }

#endif