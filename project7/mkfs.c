#include "mkfs.h"
#include "pack.h"
#include "block.h"
#include "image.h"
#include "inode.h"
#include "pack.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void mkfs(void){
    unsigned char buf[BLOCK_SIZE] = {0};

    for (int i = 0; i < NUM_OF_BLOCKS; i++) {
        write(image_fd, buf, BLOCK_SIZE);
    }

    for (int i = 0; i < 7; i++) {
        alloc();
    }

    struct inode *new_inode = ialloc();
    int new_data_block = alloc();

    new_inode->flags = FLAG_INIT;
    new_inode->size = ENTRY_SIZE * 2;
    new_inode->block_ptr[0] = new_data_block;

    unsigned char block[BLOCK_SIZE] = {0};

    char *dot = (char *)block + 2;
    char *double_dot = (char *)block + INODE_NUM_SIZE + ENTRY_SIZE;

    write_u16(block, new_inode->inode_num);
    write_u16(block + ENTRY_SIZE, new_inode->inode_num);

    strcpy(dot, ".");
    strcpy(double_dot, "..");
    bwrite(new_data_block, block);

    iput(new_inode);
}
