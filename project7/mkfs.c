#include "mkfs.h"
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

    for (int i=0; i<7; i++){
        alloc();
    }

    struct inode *new_inode = ialloc();
    int new_data_block = alloc();

    new_inode->flags = FLAG_INIT;
    new_inode->size = ENTRY_SIZE * 2;
    new_inode->block_ptr[0] = new_data_block;

    unsigned char block[BLOCK_SIZE] = {0};

    char *dot = (char *)block + 2;
    char *double_dot = (char *)block +INODE_SIZE + ENTRY_SIZE;

    write_u16(block, new_inode->inode_num);
    write_u16(block, new_inode->inode_num);

    strcpy(dot, ".");
    strcpy(double_dot, "..");
    printf("__block[2]: %d\n", block[2]);
    bwrite(new_data_block, block);

    iput(new_inode);
}

// void mkfs(void){
//     unsigned char buf[BLOCK_SIZE] = {0};

//     for (int i=0; i<NUM_OF_BLOCKS; i++){
//         write(image_fd, buf, BLOCK_SIZE);
//     }

//     for (int i=0; i<7; i++){
//         alloc();
//     }

//     // 1. call ialloc to get new inode
//     struct inode *in;
//     in = ialloc();
//     // 2. call alloc to get new data block
//     int block_num = alloc();
//     // 3. initialize inode returned from step 1. 
//     in->flags = 2;
//     in->size = 64;
//     in->block_ptr[0] = block_num;
//     // 4. make an unsigned char block[BLOCK_SIZE] to populate with new directory info
//     unsigned char block[BLOCK_SIZE];

//     // 5. Add directory entries
//     write_u16(block, in->inode_num);
//     char* ptr = (char*)block + 2;
//     strcpy(ptr, ".");
//     ptr = (char*)block + 18;
//     strcpy(ptr, "..");
//     printf("__block[2]: %d\n", block_num);
//     // 6. write the directory data block with bwrite
//     bwrite(block_num, block);
//     // 7. call iput to write inode to disk and clear incore 
//     iput(block);
// }