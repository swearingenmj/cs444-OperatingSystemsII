#include "mkfs.h"
#include "block.h"
#include "image.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void mkfs(void){
    unsigned char buf[BLOCK_SIZE] = {0};

    for (int i=0; i<NUM_OF_BLOCKS; i++){
        write(image_fd, buf, BLOCK_SIZE);
    }

    for (int i=0; i<7; i++){
        alloc();
    }

    // 1. call ialloc to get new inode
    // 2. call alloc to get new data block
    // 3. initialize inode returned from step 1. 
    // 4. make an unsigned char block[BLOCK_SIZE] to populate with new directory info
    // 5. Add directory entries
    // 6. write the directory data block with bwrite
    // 7. call iput to write inode to disk and clear incore 
}