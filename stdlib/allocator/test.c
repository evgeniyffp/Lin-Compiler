/*
 * Main.c created by evgeniyffp
*/

// ReSharper disable CppDFAMemoryLeak

#include <stdio.h>

#include "allocate.h"

int main(int argc, char** argv) {
    printf("Start:\t\n\r");
    init_alloc();

    int* array1 = (int*)alloc(10 * sizeof(int));
    int* array2 = (int*)alloc(100 * sizeof(int));
    int* array3 = (int*)alloc(1000 * sizeof(int));
    int* array4 = (int*)alloc(10000 * sizeof(int));

    for (size_t i = 0; i < 10; ++i) {
        printf("%d ", array1[i]);
    }
    printf("\n");

    for (size_t i = 0; i < 100; ++i) {
        printf("%d ", array2[i]);
    }
    printf("\n");

    for (size_t i = 0; i < 1000; ++i) {
        printf("%d ", array3[i]);
    }
    printf("\n");

    for (size_t i = 0; i < 10000; ++i) {
        printf("%d ", array4[i]);
    }
    printf("\n");

    free_alloc(array1);
    free_alloc(array3);
    free_alloc(array2);
    free_alloc(array4);

    deinit_alloc();
    return 0;
}