#include "inode.h"
#include "free.h"
#include "image.h"
#include "block.h"
#include "pack.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

void fill_incore_array(void) {
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        incore[i].ref_count = 1;
    }
}

void empty_incore_array(void) {
    memset(incore, 0, sizeof incore);
}

struct inode *ialloc(void) {
    unsigned char inode_map[BLOCK_SIZE] = {0};
    
    bread(INODE_MAP_NUM, inode_map);
    
    int free_inode_num = find_free(inode_map);
    
    set_free(inode_map, free_inode_num, SET_UNAVAILABLE);
    
    bwrite(INODE_MAP_NUM, inode_map);
    
    if (free_inode_num == -1) {
        return NULL;
    }

    struct inode *incore_inode = iget(free_inode_num);
    
    if (incore_inode == NULL) {
        return NULL;
    }

    // struct inode *incore_inode = iget(free_inode_num);
    
    incore_inode->size = 0;
    incore_inode->owner_id = 0;
    incore_inode->permissions = 0;
    incore_inode->flags = 0;
    
    for (int i = 0; i < INODE_PTR_COUNT; i++) {
        incore_inode->block_ptr[i] = 0;
    }

    incore_inode->inode_num = free_inode_num;
    
    write_inode(incore_inode);
    
    return incore_inode;
}

struct inode *find_incore_free(void) {
    int free_incore_index = NODE_MIA;
    
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        if(incore[i].ref_count == 0) {
            free_incore_index = i;
            break;
        }
    }

    return (free_incore_index != NODE_MIA) ? &incore[free_incore_index] : NULL;
}

struct inode *find_incore(unsigned int inode_num) {
    int incore_index = NODE_MIA;
    
    for (int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        if(incore[i].inode_num == inode_num && incore[i].ref_count > 0) {
            incore_index = i;
            break;
        }
    }

    return (incore_index != NODE_MIA) ? &incore[incore_index] : NULL;
}

void read_inode(struct inode *in, int inode_num) {
    // map inode number to block number and byte offset
    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    // read data from inode with block_num on disk into inode_block
    unsigned char inode_block[BLOCK_SIZE];
    bread(block_num, inode_block);

    // read inode_block data into block
    in->size = read_u32(inode_block + block_offset_bytes);
    in->owner_id = read_u16(inode_block + block_offset_bytes + OWNER_ID_OFFSET);
    in->permissions = read_u8(inode_block + block_offset_bytes + PERMISSIONS_OFFSET);
    in->flags = read_u8(inode_block + block_offset_bytes + FLAGS_OFFSET);
    in->link_count = read_u8(inode_block + block_offset_bytes + LINK_COUNT_OFFSET);
    
    for (int i = 0; i < 16; i++) {
        int offset_index = (i*PER_BLOCK_PTR_OFFSET) + BLOCK_PTR_OFFSET;
        in->block_ptr[i] = read_u16(inode_block + block_offset_bytes + offset_index);
    }
}

void write_inode(struct inode *in) {
    // map inode_num to block and offset
    int block_offset = in->inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    // read the data from disk into a block
    unsigned char inode_block[BLOCK_SIZE];
    bread(in->inode_num, inode_block);

    // pack (write) to the disk from in
    write_u32(inode_block + block_offset_bytes, in->size);
    write_u16(inode_block + block_offset_bytes + OWNER_ID_OFFSET, in->owner_id);
    write_u8(inode_block + block_offset_bytes + PERMISSIONS_OFFSET, in->permissions);
    write_u8(inode_block + block_offset_bytes + FLAGS_OFFSET, in->flags);
    write_u8(inode_block + block_offset_bytes + LINK_COUNT_OFFSET, in->link_count);
    
    for (int i = 0; i < 16; i++) {
        int offset_index = (i*PER_BLOCK_PTR_OFFSET) + BLOCK_PTR_OFFSET;
        write_u16(inode_block + block_offset_bytes + offset_index, in->block_ptr[i]);
    }

    // write data from block to disk
    bwrite(in->inode_num, inode_block);
}

struct inode *iget(int inode_num) {
    struct inode *incore_node;

    if (find_incore(inode_num) != NULL) {
        incore_node = find_incore(inode_num);
        incore_node->ref_count++;
        return incore_node;
    } 
    
    if (find_incore_free() == NULL) {
        return NULL;
    } else {
        incore_node = find_incore_free();
        
        read_inode(incore_node, inode_num);
        
        incore_node->ref_count = 1;
        incore_node->inode_num = inode_num;
        
        return incore_node;
    }
}

void iput(struct inode *in) {
    if (in->ref_count == 0) {
        return;
    }

    in->ref_count--;
    
    if (in->ref_count == 0) {
        write_inode(in);
    }
}
