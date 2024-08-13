# memory-allocator in C

## Test
- Run the following command to test:
```terminal
./bin/test
```
- If changes are made to `mem_alloc.c`, compile for testing using:
```terminal
gcc -o ./bin/test test.c mem_alloc.c -pthread
```

## Virtual Address Space

- Text section: Holds the binary code that the processor executes.
- Data section: Stores static data that is initialized with non-zero values.
- BSS (Block Started by Symbol): Contains static data that is initialized to zero. Any uninitialized static data in the program is set to zero and placed here.
- Heap: Used for storing dynamically allocated data.
- Stack: Contains automatic variables, function arguments, and a copy of the base pointer.

![memlayout](https://github.com/user-attachments/assets/e4401f77-8aeb-4be7-b725-75eaf56aaf80)

##
