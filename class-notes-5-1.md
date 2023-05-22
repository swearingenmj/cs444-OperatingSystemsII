# OS II Class Notes


# Exploration 5.1: Device I/O

What are some benefits of abstraction layers when it comes to hardware support in the OS?

Some of the benefits of abstraction layers are that they enhance portability by shielding apps from hardware devices. They also facilitate hardware compatibility by providing a standardized interface. Lastly, they increase maintainability by isolating hardware-specific code.

If you switched from PIO to DMA-with-interrupts, how would that potentially improve performance?

Switching from PIO to DMA-with-interrupts improves performance because DMA enables the hardware device to directly access the memory and transfer data without CPU intervention. Also, DMA interrupts allows the CPU to be notified when a data transfer operation is complete.

Why might you prefer memory-mapped I/O to port-based I/O?

Memory-mapped I/O might be preferrable because it allows devices to be accessed using the same instructions and mechanisms as regular memory making it easier to program. It also has a more consistent view of the system's memory. Lastly, memory-mapped I/O makes it data transfers faster by leveraging the CPU's memory access mechanisms. 


# Exploration 5.2: File System Implementation

What's in the inode?

An inode contains metadata about a file such as the file's size, permissions, timestamps, ownership, and file type. It also contains a list of block numbers that point to the blocks that contain the file's data.

We describe a free block bitmap in this exploration. What other ways might we represent free blocks in the system?

I think another way to represent free blocks would be to use a linked list. Each block would contain a pointer to the next free block. This would be similar to the linked list implementation of the free list in the buddy system.

Another way to represent free blocks could be to use grouping to represent a range of free blocks using start and end block numbers. 

Why would not having a buffer cache make things run more slowly?

Not having a buffer cache might make things run more slowly because it would require more disk accesses. The buffer cache temporarily stores data from disk in memory so that it can be accessed more quickly. Without a buffer cache, the OS would have to read from disk every time a file is accessed.
