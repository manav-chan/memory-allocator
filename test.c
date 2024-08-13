#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mem_alloc.h"

extern void* malloc(size_t);
extern void free(void*);
extern void* calloc(size_t, size_t);
extern void* realloc(void*, size_t);

void test_malloc() {
    int *arr;
    printf("Testing malloc...\n");
    arr = (int*)malloc(10 * sizeof(int));
    if (arr != NULL) {
        printf("malloc: Success\n");
    } else {
        printf("malloc: Failed\n");
    }
    free(arr);
}

void test_free() {
    int *arr;
    printf("Testing free...\n");
    arr = (int*)malloc(10 * sizeof(int));
    if (arr != NULL) {
        free(arr);
        printf("free: Success\n");
    } else {
        printf("free: Failed\n");
    }
}

void test_calloc() {
    int *arr, success, i;
    printf("Testing calloc...\n");
    arr = (int*)calloc(10, sizeof(int));
    if (arr != NULL) {
        success = 1;
        for (i = 0; i < 10; i++) {
            if (arr[i] != 0) {
                success = 0;
                break;
            }
        }
        if (success) {
            printf("calloc: Success\n");
        } else {
            printf("calloc: Failed\n");
        }
        free(arr);
    } else {
        printf("calloc: Failed\n");
    }
}

void test_realloc() {
    int *arr, *new_arr, i, success;
    printf("Testing realloc...\n");
    arr = (int*)malloc(10 * sizeof(int));
    if (arr != NULL) {
        for (i = 0; i < 10; i++) {
            arr[i] = i;
        }
        new_arr = (int*)realloc(arr, 20 * sizeof(int));
        if (new_arr != NULL) {
            success = 1;
            for (i = 0; i < 10; i++) {
                if (new_arr[i] != i) {
                    success = 0;
                    break;
                }
            }
            if (success) {
                printf("realloc: Success\n");
            } else {
                printf("realloc: Failed\n");
            }
            free(new_arr);
        } else {
            printf("realloc: Failed\n");
        }
    } else {
        printf("malloc for realloc: Failed\n");
    }
}

int main() {
    test_malloc();
    test_free();
    test_calloc();
    test_realloc();
    return 0;
}