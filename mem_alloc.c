#include <stdio.h>
#include <pthread.h>

void* malloc(size_t);
header_t* get_free_block(size_t);
void free(void*);
void* calloc(size_t, size_t);
void* realloc(void*, size_t);

/*
store size of allocated memory to free it later.
memory at end of heap can only be released therefore memory between heap will be freed to be used by other mallocs.
store size and whether memory is free or not in header.
*/

typedef char ALIGN[16];

// union is used such that the maximum size of header remains to be 16 bytes.
union header {
    struct {
        size_t size;
        unsigned int is_free;
        union header *next; // linked list to store next block if non contiguous memory is allocated.
    } s;
    ALIGN stub;
};
typedef union header header_t;

// head and tail pointers to keep track of memory allocated.
header_t *head, *tail;

// to prevent deadlocks, implementing locking mechanism so no 2 threads can concurrently access memory
pthread_mutex_t global_malloc_lock;

/* 
Calling sbrk(0) gives the current address of program break.
Calling sbrk(x) with a positive value increments brk by x bytes, as a result allocating memory.
Calling sbrk(-x) with a negative value decrements brk by x bytes, as a result releasing memory.
On failure, sbrk() returns (void*) -1.
*/

void* malloc(size_t size)
{
	size_t total_size;
	void *block;
	header_t *header;
    
	if (!size)
		return NULL;
	pthread_mutex_lock(&global_malloc_lock);
	header = get_free_block(size);
	if (header) {
		header->s.is_free = 0;
		pthread_mutex_unlock(&global_malloc_lock);
		return (void*)(header + 1);
	}
	total_size = sizeof(header_t) + size;
	block = sbrk(total_size);
	if (block == (void*) -1) {
		pthread_mutex_unlock(&global_malloc_lock);
		return NULL;
	}
	header = block;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = NULL;
	if (!head)
		head = header;
	if (tail)
		tail->s.next = header;
	tail = header;
	pthread_mutex_unlock(&global_malloc_lock);
	return (void*)(header + 1);
}

// first fit memory allocation algorithm
header_t* get_free_block(size_t size)
{
	header_t *curr = head;

	while(curr) {
		if (curr->s.is_free && curr->s.size >= size)
			return curr;
		curr = curr->s.next;
	}
	return NULL;
}

// if block at end of heap, release it to OS otherwise mark it as free.
void free(void *block)
{
	header_t *header, *tmp;
	void *programbreak;

	if (!block)
		return;
	pthread_mutex_lock(&global_malloc_lock);
	header = (header_t*)block - 1;

	programbreak = sbrk(0);
	if ((char*)block + header->s.size == programbreak) {
		if (head == tail) {
			head = tail = NULL;
		} else {
			tmp = head;
			while (tmp) {
				if(tmp->s.next == tail) {
					tmp->s.next = NULL;
					tail = tmp;
				}
				tmp = tmp->s.next;
			}
		}
		sbrk(0 - sizeof(header_t) - header->s.size);
		pthread_mutex_unlock(&global_malloc_lock);
		return;
	}
	header->s.is_free = 1;
	pthread_mutex_unlock(&global_malloc_lock);
}

void* calloc(size_t num, size_t nsize)
{
	size_t size;
	void *block;

	if (!num || !nsize)
		return NULL;
	size = num * nsize;

    // check mul overflow 
	if (nsize != size / num)
		return NULL;

	block = malloc(size);
	if (!block)
		return NULL;
	memset(block, 0, size);
	return block;
}

void* realloc(void *block, size_t size)
{
	header_t *header;
	void *ret;

	if (!block || !size)
		return malloc(size);
	header = (header_t*)block - 1;
	if (header->s.size >= size)
		return block;
	ret = malloc(size);
	if (ret) {
		
		memcpy(ret, block, header->s.size);
		free(block);
	}
	return ret;
}