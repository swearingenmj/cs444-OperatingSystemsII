#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "block.h"
#include "image.h"
#include "free.h"

unsigned char *bread(int block_num, unsigned char *block) {
    lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET);
    read(image_fd, block, BLOCK_SIZE);
    return block;
}

void bwrite(int block_num, unsigned char *block) {
    lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET);
    write(image_fd, block, BLOCK_SIZE);
}

int alloc(void) {
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
