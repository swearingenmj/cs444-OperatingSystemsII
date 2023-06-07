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
    // find the directory path that will contain the new directory - use dirbasename.c
    char dirname[strlen(path)];
    get_dirname(path, dirname);
    printf("__dirname: :%s\n", dirname);

    // find the new directory name from the path - use dirbasename.c
    char basename[strlen(path)];
    get_basename(path, basename);
    printf("__basename: :%s\n", basename);


    // find the inode for the parent directory that will hold the new entry (namei())
    struct inode *parent_inode = namei(dirname);
    printf("__parent dir size: %d, block[0]: %d, inode_num: %d\n", parent_inode->size, parent_inode->block_ptr[0], parent_inode->inode_num);
    // create a new inode for the new directroy (ialloc())
    struct inode *new_inode = ialloc();

    // create a new data block for the new directory entries (alloc())
    int new_data_block_num = alloc();
    

    // create a new block-sized array for the new directory data block and initialize it . and .. files.
        // the . should contain the new directory's inode number
        // the .. should contain the parent director's inode number
    unsigned char new_data_block[BLOCK_SIZE] = {0};
    char *dot = (char *)new_data_block + 2;
    char *double_dot = (char *)new_data_block + INODE_NUM_SIZE + ENTRY_SIZE;

    write_u16(new_data_block, new_inode->inode_num);
    write_u16(new_data_block + ENTRY_SIZE, parent_inode->inode_num);

    strcpy(dot, ".");
    strcpy(double_dot, "..");


    // initialize the new directory incore inode with a proper size and other fields
    new_inode->flags = FLAG_INIT;
    new_inode->size = ENTRY_SIZE * 2;
    new_inode->block_ptr[0] = new_data_block_num;

    printf("__new dir size: %d, block[0]: %d, inode_num: %d\n", new_inode->size, new_inode->block_ptr[0], new_inode->inode_num);



    // write the new directory data block to dish (bwrite())
    bwrite(new_data_block_num, new_data_block);

    printf("__new_data_block_num: %d\n", new_data_block_num);
    
    // from the parent directory inode, find the block that will contain the new directory entry (using size and block_ptr fields)
    
    int block_ptr_index = parent_inode->size / BLOCK_SIZE;
    int parent_block_num = parent_inode->block_ptr[block_ptr_index];
    int parent_block_offset = parent_inode->size % BLOCK_SIZE;
    unsigned char parent_block[BLOCK_SIZE] = {0};
    // read that block into memory unless you're creating a new one (bread()) and add the new directory entry to it
    printf("__parent_inode->size: %d, block_ptr_index: %d, parent_block_num: %d\n", parent_inode->size, block_ptr_index, parent_block_num);
    if (parent_block_offset == 0) {
        // block is full
        // create new one
        parent_block_num = alloc();
    } else { 
        bread(parent_block_num, parent_block);
    }

    write_u16(parent_block + parent_block_offset, new_inode->inode_num);
    // add the new directory entry to it
    char *new_dir_entry = (char *)parent_block + parent_block_offset + 2;
    strcpy(new_dir_entry, basename);

    // write that block to disk (bwrite())
    bwrite(parent_block_num, parent_block);

    // update the parent directory's size field (which should increase by 32)
    parent_inode->size += ENTRY_SIZE;

    // release the new directory's incore inode (iput())
    iput(new_inode);

    // release the parent directory's incore inode (iput())
    iput(parent_inode);

    return 0;
}
