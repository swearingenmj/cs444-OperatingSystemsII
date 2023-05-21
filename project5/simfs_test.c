#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "block.h"
#include "ctest.h"
#include "free.h"
#include "image.h"
#include "inode.h"
#include "mkfs.h"

void test_image_open(void) {
    CTEST_ASSERT(image_open("test_file_open", 1) != -1, "testing file opened successfully");
    CTEST_ASSERT(image_open("./", 1) == -1, "testing bad file path/fails to open");
}

void test_image_close(void) {
    CTEST_ASSERT(image_close() == -1, "testing file closed successfully");
}

void test_bread_bwrite(void) {
    image_open("test_file_read_write", 1);
    mkfs();
    unsigned char test_block_write[BLOCK_SIZE] = "test_block_write";
    unsigned char *test_block_read = malloc(BLOCK_SIZE);
    CTEST_ASSERT(memcmp(test_block_write, test_block_read, BLOCK_SIZE) != 0, "testing block read and write are not equal");
    bwrite(DATA_MAP_BLOCK, test_block_write);
    bread(DATA_MAP_BLOCK, test_block_read);
    CTEST_ASSERT(memcmp(test_block_write, test_block_read, BLOCK_SIZE) == 0, "testing block read and write are the same");
    free(test_block_read);
}

void test_find_set_free(void) {
    unsigned char test_block_free[BLOCK_SIZE] = {2};
    unsigned char test_compare_block[BLOCK_SIZE] = {0};
    set_free(test_block_free, 1, 0);
    CTEST_ASSERT(memcmp(test_block_free, test_compare_block, BLOCK_SIZE) == 0, "testing freed block by comparison with empty block");
    CTEST_ASSERT(find_free(test_block_free) == 0, "testing find free block returns 0");

    set_free(test_block_free, 0, 1);
    unsigned char test_compare_block_2[BLOCK_SIZE] = {1};
    CTEST_ASSERT(memcmp(test_block_free, test_compare_block_2, BLOCK_SIZE) == 0, "testing set block by comparison with test block of value 1");
    CTEST_ASSERT(find_free(test_block_free) == 1, "testing find free block returns 1");
}

void test_alloc(void) {
    unsigned char *test_block_alloc = malloc(BLOCK_SIZE);
    image_open("test_file_alloc", 1);
    mkfs();
    int test_alloc = alloc();
    CTEST_ASSERT(test_alloc == 7, "testing alloc returns 7 on newly created filesystem");
    test_alloc = alloc();
    CTEST_ASSERT(test_alloc == 8, "testing alloc increased by 1 when called again");    
    
    unsigned char test_compare_block[BLOCK_SIZE] = {255,1};
    bread(DATA_MAP_BLOCK, test_block_alloc);
    CTEST_ASSERT(memcmp(test_block_alloc, test_compare_block, BLOCK_SIZE) == 0, "testing alloc writes and reads to disk");
    free(test_block_alloc);
}

void test_ialloc(void) {
    unsigned char *test_block_ialloc = malloc(BLOCK_SIZE);
    image_open("test_file_ialloc", 1);
    mkfs();
    int test_ialloc = ialloc();
    CTEST_ASSERT(test_ialloc == 0, "testing ialloc returns 7 on newly created filesystem");
    test_ialloc = ialloc();
    CTEST_ASSERT(test_ialloc == 1, "testing ialloc increased by 1 when called again");    
    
    unsigned char test_compare_block[BLOCK_SIZE] = {3};
    bread(INODE_MAP_BLOCK, test_block_ialloc);
    CTEST_ASSERT(memcmp(test_block_ialloc, test_compare_block, BLOCK_SIZE) == 0, "testing ialloc writes and reads to disk");
    free(test_block_ialloc);
}

void test_mkfs(void) {
    image_open("test_file_mkfs", 1);
    mkfs();
    int test_find_end = lseek(image_fd, 0, SEEK_END);
    CTEST_ASSERT(test_find_end == 4096*1024, "testing mkfs creates a file of size 4MB");
}

int main() {
    CTEST_VERBOSE(1);

    test_image_open();
    test_image_close();
    test_bread_bwrite();
    test_find_set_free();
    test_alloc();
    test_ialloc();
    test_mkfs();

    CTEST_RESULTS();
    CTEST_EXIT();
}
