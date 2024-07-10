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

extern void __printf(char*);
extern void __exit(int);
extern void* __brk(void*);

#define _DEFAULT_STACKSIZE 1048576 // = 1mb
#define assert(x, y) { if (x) { __printf(y); __exit(1); } }

struct MetaData {
    struct MetaData* _next;
    struct MetaData* _prev;
    size_t _size;
    bool _isUse;
    bool _isAlocated;
};

struct MainData {
    struct MetaData* _start;
    size_t _size;
};

static void* endheap;
#define _MainData ((struct MainData*)endheap)

void __malloc_init() {
    assert(sizeof(byte_t) != 1, "init_alloc(): size of byte_t must be 1 byte\n");

    endheap = __brk(NULL);

    assert(endheap == NULL, "init_alloc(): end of heap == null\n");

    __brk((byte_t*)endheap + _DEFAULT_STACKSIZE);

    _MainData->_start = NULL;
    _MainData->_size = _DEFAULT_STACKSIZE;
}

void __malloc_deinit() {
    __brk(endheap);
}

void* __malloc(size_t size) {
    byte_t* ptr = (byte_t*)endheap + sizeof(struct MainData);

#define mdata ((struct MetaData*)ptr)

    if (_MainData->_start == NULL) {
        _MainData->_start = (struct MetaData*)ptr;
        mdata->_next = NULL;
        mdata->_prev = NULL;
        mdata->_size = size;
        mdata->_isAlocated = true;
        mdata->_isUse = true;
        return (void*)(ptr + sizeof(struct MetaData));
    }

    while (true) {
        if (mdata->_next == NULL) {
            mdata->_next = (struct MetaData*)(ptr + mdata->_size + sizeof(struct MetaData));
            mdata->_next->_prev = mdata;
            mdata->_next->_size = size;
            mdata->_next->_isAlocated = true;
            mdata->_next->_isUse = true;

            return (void*)((char*)mdata->_next + sizeof(struct MetaData));
        }

        ptr = (byte_t*)mdata->_next;
    }
}

void __free(void* alocated_ptr) {
    assert(alocated_ptr == NULL, "free(): poiner is null\n");
    byte_t* ptr = (byte_t*)alocated_ptr;
    ptr -= sizeof(struct MetaData);

#define mdata ((struct MetaData*)ptr)

    mdata->_isUse = false;
    if (mdata->_prev != NULL && !mdata->_prev->_isUse) {
        mdata->_prev->_size += sizeof(struct MetaData) + mdata->_size;
        mdata->_prev->_next = mdata->_next;
        if (mdata->_next != NULL) {
            mdata->_next->_prev = mdata->_prev;
        }
        ptr = (byte_t*)mdata->_prev; // mdata = mdata->_prev
    }
    if (mdata->_next != NULL && !mdata->_next->_isUse) {
        mdata->_size += sizeof(struct MetaData) + mdata->_next->_size;
        mdata->_next = mdata->_next->_next;
        mdata->_next->_prev = mdata;
    }
    mdata->_isUse = false;
}