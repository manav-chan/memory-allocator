#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <stddef.h>

void* malloc(size_t size);
void free(void* block);
void* calloc(size_t num, size_t nsize);
void* realloc(void* block, size_t size);

#endif // MEM_ALLOC_H