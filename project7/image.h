#ifndef IMAGE_H
#define IMAGE_H

#define SUPERBLOCK_NUM 0
#define INODE_MAP_NUM 1
#define DATA_MAP_NUM 2
#define BLOCK_SIZE 4096
#define NUM_OF_BLOCKS 1024
#define SET_UNAVAILABLE 1
#define SET_AVAILABLE 0
#define DO_TRUNCATE 1
#define DO_NOT_TRUNCATE 0
#define DIRECTORY_INODE_NUM 0

// for testing purposes
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

int image_open(char *filename, int truncate);
int image_close(void);

extern int image_fd;

#endif
