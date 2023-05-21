#include "mkfs.h"
#include "block.h"

void mkfs(void) {
    unsigned char *block = malloc(BLOCK_SIZE);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write(i, block, 4096);
    }

    for (int i = 0; i < 7; i++) {
        alloc();
    }
    free(block);
}
