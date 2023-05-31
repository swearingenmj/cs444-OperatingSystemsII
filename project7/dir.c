#include "dir.h"
#include "inode.h"
#include "block.h"
#include "pack.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct directory *directory_open(int inode_num){
    printf("_directory open\n");
    struct inode *node; 
    node = iget(inode_num);
    
    if (node == NULL){
        printf("__node doesn't exist\n");
        return NULL;
    }

    struct directory *dir = malloc(sizeof(struct directory));
    dir->inode = node;
    dir->offset = 0;

    return dir;
}

int directory_get(struct directory *dir, struct directory_entry *ent){
    printf("_directory get\n");
    printf("__directory offset: %d,", dir->offset);
    
    printf(" directory size: %d\n", dir->inode->size);
    if (dir->offset >= dir->inode->size){
        return -1;
    }

    int data_block_index = dir->offset / 4096;
    int data_block_num = dir->inode->block_ptr[data_block_index];
    
    printf("__data block num: %d\n", data_block_num);
    
    unsigned char block[BLOCK_SIZE] = {0};
    bread(data_block_num, block);

    printf("__read into block: %d\n", block[2]);
   
    int offset_in_block = dir->offset % 4096;
    int inode_num = read_u16(block + offset_in_block);
    ent->inode_num = inode_num;
    printf("__inode num: %d\n", inode_num);

    char* read_block_name_ptr = (char*)block + 2;
    strcpy(ent->name, read_block_name_ptr);
    printf("__name: %s\n", read_block_name_ptr);

    dir->offset+=32;
    return 0;
}

void directory_close(struct directory *d){
    iput(d->inode);
    free(d);
}
