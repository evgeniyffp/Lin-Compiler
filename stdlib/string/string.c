#include "../core.h"
#include "../allocator/allocate.h"

char* __strcpy(const char* src) {
    char* dest = NULL;

    const size_t src_size = __strlen(src);

    dest = __malloc(src_size * sizeof(char));

    for (size_t i = 0; i < src_size; ++i) {
        dest[i] = src[i];
    }

    return dest;
}

char* __strcat(const char* left, const char* right) {
    size_t left_size = __strlen(left);
    size_t right_size = __strlen(right);

    char* str = __malloc((left_size + right_size) * sizeof(char));

    for (size_t i = 0; i < left_size; ++i) {
        str[i] = left[i];
    }

    for (size_t i = 0; i < right_size; ++i) {
        str[i + left_size] = right[i];
    }

    return str;
}

