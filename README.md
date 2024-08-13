# memory-allocator in C

## Virtual Address Space

- Text section: Holds the binary code that the processor executes.
- Data section: Stores static data that is initialized with non-zero values.
- BSS (Block Started by Symbol): Contains static data that is initialized to zero. Any uninitialized static data in the program is set to zero and placed here.
- Heap: Used for storing dynamically allocated data.
- Stack: Contains automatic variables, function arguments, and a copy of the base pointer.

![memlayout](https://github.com/user-attachments/assets/e4401f77-8aeb-4be7-b725-75eaf56aaf80)

Now if we want to allocate more memory in the heap, we need to request the system to increment brk. Similarly, to release memory we need to request the system to decrement brk.

Assuming we run Linux (or a Unix-like system), we can make use of sbrk() system call that lets us manipulate the program break.

Calling sbrk(0) gives the current address of program break.
Calling sbrk(x) with a positive value increments brk by x bytes, as a result allocating memory.
Calling sbrk(-x) with a negative value decrements brk by x bytes, as a result releasing memory.
On failure, sbrk() returns (void*) -1.
