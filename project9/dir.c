#include "dir.h"
#include "inode.h"
#include "block.h"
#include "pack.h"
#include "mkfs.h"
#include "namei.h"
#include "dirbasename.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 

struct directory *directory_open(int inode_num) {
    struct inode *node; 
    node = iget(inode_num);
    
    if (node == NULL) {
        return NULL;
    }

    struct directory *dir = malloc(sizeof(struct directory));
    dir->inode = node;
    dir->offset = 0;

    return dir;
}

int directory_get(struct directory *dir, struct directory_entry *ent) {  
    if (dir->offset >= dir->inode->size) {
        return -1;
    }

    int data_block_index = dir->offset / 4096;
    int data_block_num = dir->inode->block_ptr[data_block_index];
    
    unsigned char block[BLOCK_SIZE] = {0};
    bread(data_block_num, block);

    int offset_in_block = dir->offset % 4096;
    int inode_num = read_u16(block + offset_in_block);
    
    ent->inode_num = inode_num;

    char* read_block_name_ptr = (char*)block + offset_in_block + INODE_NUM_SIZE;
    strcpy(ent->name, read_block_name_ptr);

    dir->offset+=32;
    return 0;
}

void directory_close(struct directory *d) {
    iput(d->inode);
    
    free(d);
}

int directory_make(char *path) {
    char dirname[strlen(path)];
    get_dirname(path, dirname);

    char basename[strlen(path)];
    get_basename(path, basename);

    struct inode *parent_inode = namei(dirname);
    struct inode *new_inode = ialloc();
    int new_data_block_num = alloc();

    unsigned char new_data_block[BLOCK_SIZE] = {0};
    char *dot = (char *)new_data_block + 2;
    char *double_dot = (char *)new_data_block + INODE_NUM_SIZE + ENTRY_SIZE;

    write_u16(new_data_block, new_inode->inode_num);
    write_u16(new_data_block + ENTRY_SIZE, parent_inode->inode_num);

    strcpy(dot, ".");
    strcpy(double_dot, "..");

    new_inode->flags = FLAG_INIT;
    new_inode->size = ENTRY_SIZE * 2;
    new_inode->block_ptr[0] = new_data_block_num;

    bwrite(new_data_block_num, new_data_block);

    int block_ptr_index = parent_inode->size / BLOCK_SIZE;
    int parent_block_num = parent_inode->block_ptr[block_ptr_index];
    int parent_block_offset = parent_inode->size % BLOCK_SIZE;
    unsigned char parent_block[BLOCK_SIZE] = {0};

    if (parent_block_offset == 0) {
        parent_block_num = alloc();
    } else { 
        bread(parent_block_num, parent_block);
    }

    write_u16(parent_block + parent_block_offset, new_inode->inode_num);

    char *new_dir_entry = (char *)parent_block + parent_block_offset + 2;
    strcpy(new_dir_entry, basename);

    bwrite(parent_block_num, parent_block);

    parent_inode->size += ENTRY_SIZE;

    iput(new_inode);
    iput(parent_inode);

    return 0;
}
