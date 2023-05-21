#include "inode.h"
#include "free.h"
#include "image.h"
#include "block.h"
#include "pack.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

void fill_incore_array(void){
    for(int i=0; i<MAX_SYS_OPEN_FILES; i++){
        incore[i].ref_count = 1;
    }
}

void empty_incore_array(void){
    memset(incore, 0, sizeof incore);
}

struct inode *ialloc(void){
    printf("_ialloc");
    unsigned char inode_map[BLOCK_SIZE] = {0};
    bread(INODE_MAP_NUM, inode_map);
    int free_inode_num = find_free(inode_map);
    set_free(inode_map, free_inode_num, SET_UNAVAILABLE);
    bwrite(INODE_MAP_NUM, inode_map);
    if(free_inode_num == -1){
        return NULL;
    }
    if(iget(free_inode_num)==NULL){
        return NULL;
    }
    struct inode incore_inode = *iget(free_inode_num);
    incore_inode.size = 0;
    incore_inode.owner_id = 0;
    incore_inode.permissions = 0;
    incore_inode.flags = 0;
    for(int i=0; i<INODE_PTR_COUNT; i++){
        incore_inode.block_ptr[i] = 0;
    }
    
    incore_inode.inode_num = free_inode_num;
    write_inode(&incore_inode);
    

    return &incore_inode;
}

struct inode *find_incore_free(void){
    printf("_find_incore_free\n");
    int free_incore_index = -1;
    for(int i=0; i<MAX_SYS_OPEN_FILES; i++){
        if(incore[i].ref_count == 0){
            free_incore_index = i;
            break;
        }
    }
    return (free_incore_index != -1) ? &incore[free_incore_index] : NULL;
}

struct inode *find_incore(unsigned int inode_num){
    printf("_find_incore: %d\n", inode_num);
    int incore_index = -1;
    for(int i=0; i<MAX_SYS_OPEN_FILES; i++){
        // printf("__inode num %d, ref_count %d\n", incore[i].inode_num, incore[i].ref_count);
        if(incore[i].inode_num == inode_num & incore[i].ref_count > 0){
            incore_index = i;
            break;
        }
    }
    // printf("__found index %d\n", incore_index);
    return (incore_index != -1) ? &incore[incore_index] : NULL;
}

void read_inode(struct inode *in, int inode_num){
    printf("_read_inode inode num: %d ", inode_num);
    printf("for struct %d\n", in->inode_num);    

    // map inode number to block number and byte offset
    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    // read data from inode with block_num on disk into inode_block
    unsigned char inode_block[BLOCK_SIZE];
    bread(block_num, inode_block);

    // read inode_block data into incore struct
    in->size = read_u32(inode_block + block_offset_bytes);
    in->owner_id = read_u16(inode_block + block_offset_bytes + 4);
    in->permissions = read_u8(inode_block + block_offset_bytes + 6);
    in->flags = read_u8(inode_block + block_offset_bytes + 7);
    in->link_count = read_u8(inode_block + block_offset_bytes + 8);
    for (int i=0; i<16; i++){
        int offset_index = (i*2) + 9;
        in->block_ptr[i] = read_u16(inode_block + block_offset_bytes + offset_index);
    }
    
    // printf("in->size %d\n", in->size);
    // reads from disk, the inode at inode_num
    // places data into the struct inode *in
}

void write_inode(struct inode *in){
    printf("_write_inode\n");
    // map inode_num to block and offset
    int block_num = in->inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = block_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    // read the data from disk into a block
    unsigned char inode_block[BLOCK_SIZE];
    bread(in->inode_num, inode_block);

    // pack (write) to the disk from in
    write_u32(inode_block + block_offset_bytes, in->size);
    write_u16(inode_block + block_offset_bytes + 4, in->owner_id);
    write_u8(inode_block + block_offset_bytes + 6, in->permissions);
    write_u8(inode_block + block_offset_bytes + 7, in->flags);
    write_u8(inode_block + block_offset_bytes + 8, in->link_count);
    for (int i=0; i<16; i++){
        int offset_index = (i*2) + 9;
        write_u16(inode_block + block_offset_bytes + offset_index, in->block_ptr[i]);
    }

    // write data from block to disk
    bwrite(in->inode_num, inode_block);
    printf("__write_inode struct: %d\n", in->size);
}

struct inode *iget(int inode_num){
    printf("_iget: %d\n", inode_num);
    struct inode incore_node;
    if (find_incore(inode_num) != NULL){
        printf("__inode_num found\n");
        incore_node = *find_incore(inode_num);
        incore_node.ref_count++;
        return &incore_node;
    } 
    if (find_incore_free() == NULL) {
        printf("__no nodes :'(\n");
        return NULL;
    } else {
        printf("__getting free incore\n");
        incore_node = *find_incore_free();
        read_inode(&incore_node, inode_num);
        incore_node.ref_count = 1;
        incore_node.inode_num = inode_num;
        return &incore_node;
    }
}

void iput(struct inode *in){
    printf("_iput: %d\n", in->size);
    if (in->ref_count == 0){
        return;
    }
    in->ref_count--;
    if (in->ref_count == 0){
        write_inode(in);
    }

}
