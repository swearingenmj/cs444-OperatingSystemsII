#include "dir.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct directory *directory_open(int inode_num){

    // Use iget() to get the inode for this file.

    // If it fails, directory_open() should return NULL.

    // malloc() space for a new struct directory.

    // In the struct, set the inode pointer to point to the inode returned by iget().

    // Initialize offset to 0.

    // Return the pointer to the struct.

    return NULL;
}

int directory_get(struct directory *dir, struct directory_entry *ent){


    // 1. Check the offset against the size of the directory. If the offset is greater-than or equal-to the directory size (in its inode), we must be off the end of the directory. If so, return -1 to indicate that.

    // 2. Compute the block in the directory we need to read. The directory file itself might span multiple data blocks if there are enough entries in it. Remember that a block only holds 128 entries. (When we just create it, it will only be one block, but we might as well do this math now so it will work later.)

    // If each block is 4096 bytes, and we're at offset 4800, we must be past the end of block 0 and we're somewhere in block 1. Good old divide and modulo to the rescue!

    // data_block_index = offset / 4096;

    // Beware! this isn't the absolute block number on disk. Don't just feed this number into bread()! See the next step, below!

    // 3. We need to read the appropriate data block in so we can extract the directory entry from it.

    // But what we have, the data_block_index, is giving us the index into the block_ptr array in the directory's inode.

    // Sheesh! More indirection; there's a shocker.

    // So in order to get the block number to read, we have to look that up. Luckily, we have the inode in the struct directory * that was passed into this function:

    // data_block_num = dir->inode->block_ptr[data_block_index];

    // // and finally...

    // bread(data_block_num, block);

    // And there we've read the block containing the directory entry we want into memory.

    // Compute the offset of the directory entry in the block we just read.

    // We know the absolute offset of where we are in the file (passed in the struct directory.

    // We know the size of the block, 4096 bytes.

    // So we can compute the offset within the block:

    // offset_in_block = offset % 4096;

    // 4. Extract the directory entry from the raw data in the block into the struct directory_entry * we passed in.

    // Now we have the data in our block array, and we've computed where it starts with offset_in_block. We just have to deserialize it.

    // Use read_16() to extract the inode number and store it in ent->inode_num.

    // Use strcpy() to extract the file name and store it in ent->name. You might have to do some casting to char *.


    return 0;
}

void directory_close(struct directory *d){

    // iput() the directory's in-core inode to free it up.

    // free() the struct directory.

}
