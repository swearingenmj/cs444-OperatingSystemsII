#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mkfs.h"
#include "block.h"
#include "image.h"

void mkfs(void) {
    unsigned char *block = malloc(BLOCK_SIZE);
    for (int i = 0; i < NUM_BLOCKS; i++) {
        write(image_fd, block, 4096);
    }

    for (int i = 0; i < 7; i++) {
        alloc();
    }
    free(block);
}
