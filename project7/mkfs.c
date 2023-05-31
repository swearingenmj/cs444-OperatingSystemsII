#include "mkfs.h"
#include "pack.h"
#include "block.h"
#include "image.h"
#include "inode.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void mkfs(void){
    unsigned char buf[BLOCK_SIZE] = {0};

    for (int i = 0; i < NUM_OF_BLOCKS; i++) {
        write(image_fd, buf, BLOCK_SIZE);
    }

    for (int i=0; i<7; i++){
        alloc();
    }

    // 1. call ialloc to get new inode
    struct inode *new_inode = ialloc();

    // 2. call alloc to get new data block
    int new_data_block = alloc();

    // 3. initialize inode returned from step 1.
    new_inode->flags = FLAG_INIT;
    new_inode->size = ENTRY_SIZE;
    new_inode->block_ptr[0] = new_data_block;

    // 4. make an unsigned char block[BLOCK_SIZE] to populate with new directory info
    unsigned char block[BLOCK_SIZE] = {0};

    // 5. Add directory entries. 
    char *dot = (char *)(block);
    char *double_dot = (char *)(block + ENTRY_SIZE);

    write_u16(dot, new_inode->inode_num);
    write_u16(double_dot, new_inode->inode_num);

    strcpy(dot + INODE_SIZE, ".");
    strcpy(double_dot + INODE_SIZE + ENTRY_SIZE, "..");
    
    // 6. write the directory data block with bwrite
    bwrite(new_data_block, block);

    // 7. call iput to write inode to disk and clear incore 
    iput(new_inode);
}