
# Variable Size Allocator (VSA) in C

This project demonstrates my implementation of a Variable Size Allocator (VSA) in C.


## Features

- Memory allocation and deallocation for variable-sized memory blocks.
- Automatic defragmentation to merge consecutive free memory blocks.
- Determine the size of the largest contiguous free memory block.


## Usage

Note: by compiling the files (using makefile), a `vsa.out` file will be created, which is an executable for the testing I performed.<br>
If you want to use the VSA by itself, only download the `vsa.c` file.

1. Clone this repository.
2. Compile the files using the makefile in the terminal:
      ```bash
      make
      ```
3. Run the executable in the terminal:
      ```bash
      ./vsa.out
      ```
4. Read the comments in the `vsa.h` / `vsa.c` files to understand what each function does.



## Known Issues

The "VsaAlloc" function allocates the first available block matching the size requested, without considering better fit in next blocks.

This can result in fregmentaion, that is handled by the "Defragmentation" function to merge free blocks.
