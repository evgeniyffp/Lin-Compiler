#include "../core.h"
#include "../allocator/allocate.h"

char* __strnew(size_t size) {
    return __malloc(size * sizeof(char));
}

char* __strcpy(const char* src) {
    char* dest = NULL;

    const size_t src_size = __strlen(src);

    dest = __strnew(src_size);

    for (size_t i = 0; i < src_size; ++i) {
        dest[i] = src[i];
    }

    return dest;
}

char* __strcat(const char* left, const char* right) {
    size_t left_size = __strlen(left);
    size_t right_size = __strlen(right);

    char* str = __strnew(left_size + right_size);

    for (size_t i = 0; i < left_size; ++i) {
        str[i] = left[i];
    }

    for (size_t i = 0; i < right_size; ++i) {
        str[i + left_size] = right[i];
    }

    return str;
}

