#include "inode.h"
#include "block.h"

int ialloc(void) {
    unsigned char *data_map = malloc(BLOCK_SIZE);
    bread(DATA_MAP_BLOCK, data_map);
    int free_block = find_free(data_map);
    if (free_block == -1) {
        return -1;
    }
    set_free(data_map, free_block, 1);
    bwrite(DATA_MAP_BLOCK, data_map);
    return free_block;
}