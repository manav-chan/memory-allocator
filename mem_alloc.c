#include <stdio.h>
#include <pthread.h>

void* malloc(size_t);
header_t* get_free_block(size_t);

/* we need to store size of allocated memory as we should be able to free it later.
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

/* Calling sbrk(0) gives the current address of program break.
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