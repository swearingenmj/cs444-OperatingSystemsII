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
    CTEST_ASSERT(image_open("test_file_open_failure", 1) == -1, "testing file failed to open");
}

void test_image_close(void) {
    CTEST_ASSERT(image_close() == -1, "testing file closed successfully");
}

void test_bread_bwrite(void) {
    unsigned char test_block_write[BLOCK_SIZE] = "test_block_write";
    unsigned char *test_block_read = malloc(BLOCK_SIZE);
    CTEST_ASSERT(memcmp(test_block_write, test_block_read, BLOCK_SIZE) != 0, "testing block read and write");
    bwrite(2, test_block_write);
    bread(2, test_block_read);
    CTEST_ASSERT(memcmp(test_block_write, test_block_read, BLOCK_SIZE) == 0, "testing block read and write");
    free(test_block_read);
}

void test_find_set_free(void) {
    unsigned char *test_block_free = malloc(BLOCK_SIZE);
    set_free(test_block_free, 0, 1);
    CTEST_ASSERT(find_free(test_block_free) == 0, "testing find free and set free");
    free(test_block_free);
}

void test_alloc(void) {
    unsigned char *test_block_alloc = malloc(BLOCK_SIZE);
    image_open("test_file_alloc", 1);
    int test_alloc = alloc();
    CTEST_ASSERT(test_alloc != -1, "testing alloc");
    bread(test_alloc, test_block_alloc);
    CTEST_ASSERT(memcmp(test_block_alloc, "\0", BLOCK_SIZE) == 0, "testing alloc");
    free(test_block_alloc);
}

void test_ialloc(void) {
    unsigned char *test_block_ialloc = malloc(BLOCK_SIZE);
    image_open("test_file_ialloc", 1);
    int test_ialloc = ialloc();
    CTEST_ASSERT(test_ialloc != -1, "testing ialloc");
    bread(test_ialloc, test_block_ialloc);
    CTEST_ASSERT(memcmp(test_block_ialloc, "\0", BLOCK_SIZE) == 0, "testing ialloc");
    free(test_block_ialloc);
}

void test_mkfs(void) {
    image_open("test_file_mkfs", 1);
    mkfs();
    unsigned char *test_block_mkfs = malloc(BLOCK_SIZE);
    int test_find_end = lseek(image_fd, 0, SEEK_END);
    bread(test_find_end, test_block_mkfs);
    CTEST_ASSERT(memcmp(test_block_mkfs, "\0", BLOCK_SIZE) == 0, "testing mkfs");
    free(test_block_mkfs);
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
