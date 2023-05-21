#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "free.h"
#include "inode.h"
#include "block.h"

int ialloc(void) {
    unsigned char *data_map = malloc(BLOCK_SIZE);
    bread(INODE_MAP_BLOCK, data_map);
    int free_block = find_free(data_map);
    if (free_block == -1) {
        return -1;
    }
    set_free(data_map, free_block, 1);
    bwrite(INODE_MAP_BLOCK, data_map);
    return free_block;
}