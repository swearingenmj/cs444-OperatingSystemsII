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

    struct inode *new_inode = ialloc();
    int new_data_block = alloc();

    new_inode->flags = FLAG_INIT;
    new_inode->size = ENTRY_SIZE;
    new_inode->block_ptr[0] = new_data_block;

    unsigned char block[BLOCK_SIZE] = {0};

    char *dot = (char *)(block);
    char *double_dot = (char *)(block + ENTRY_SIZE);

    write_u16(dot, new_inode->inode_num);
    write_u16(double_dot, new_inode->inode_num);

    strcpy(dot + INODE_SIZE, ".");
    strcpy(double_dot + INODE_SIZE + ENTRY_SIZE, "..");
    
    bwrite(new_data_block, block);

    iput(new_inode);
}