#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "image.h"

int image_fd;

int image_open(char *filename, int truncate) {
    if (truncate) {
        image_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    } else {
        image_fd = open(filename, O_RDWR | O_CREAT, 0666);
    }
    return image_fd;
}

int image_close(void) {
    return close(image_fd);
}
