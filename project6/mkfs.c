#include "mkfs.h"
#include "block.h"
#include "image.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void mkfs(void) {
    unsigned char buf[BLOCK_SIZE] = {0};
    
    for (int i = 0; i < NUM_OF_BLOCKS; i++) {
        write(image_fd, buf, BLOCK_SIZE);
    }

    for (int i = 0; i < 7; i++) {
        alloc();
    }
}