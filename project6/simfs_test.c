#include "image.h"
#include "block.h"
#include "free.h"
#include "mkfs.h"
#include "ctest.h"
#include "inode.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

void test_image_open(void) {
    CTEST_ASSERT(image_open("new_file", 1) != -1, "assert image open succeeds");
    CTEST_ASSERT(image_open("/new_file", 1) == -1, "assert improper image open fails");
}

void test_image_close(void) {
    CTEST_ASSERT(image_close() == -1, "assert image close succeeds");
    image_open("test_file", 1);
    CTEST_ASSERT(image_close() != -1, "assert image close succeeds");
}

void test_bwrite_and_bread(void) {
    unsigned char write_block[BLOCK_SIZE] = "hello";
    unsigned char read_block[BLOCK_SIZE] = {0};
    CTEST_ASSERT(memcmp(write_block, read_block, BLOCK_SIZE) != 0, "assert originating buffers don't match");
    bwrite(3, write_block);
    bread(3, read_block);
    CTEST_ASSERT(memcmp(write_block, read_block, BLOCK_SIZE) == 0, "assert write and read buffers match");
}

void test_find_free(void) {
    unsigned char test_buf[BLOCK_SIZE] = {1};
    CTEST_ASSERT(find_free(test_buf) == 1, "assert find_free() returns 1 for 0b0000001");
    unsigned char test_buf_2[BLOCK_SIZE] = {191};
    CTEST_ASSERT(find_free(test_buf_2) == 6, "assert find_free() returns 6 for int 0b10111111");
    unsigned char test_buf_3[BLOCK_SIZE] = {255, 1};
    CTEST_ASSERT(find_free(test_buf_3) == 9, "assert find_free() returns 9 for 0b0000010111111111");
}

void test_set_free(void) {
    unsigned char test_block_1[BLOCK_SIZE] = {5};
    unsigned char compare_block_1[BLOCK_SIZE] = {5};
    unsigned char compare_block_1a[BLOCK_SIZE] = {4};
    CTEST_ASSERT(memcmp(test_block_1, compare_block_1, BLOCK_SIZE) == 0, "assert test and compare block match");
    
    set_free(test_block_1, 0, 0);
    CTEST_ASSERT(memcmp(test_block_1, compare_block_1a, BLOCK_SIZE) == 0, "assert set_free() clears first bit for 0b00000101");
    
    // set test block to 0b0000010100000101
    unsigned char test_block_2[BLOCK_SIZE] = {5, 5};
    unsigned char compare_block_2[BLOCK_SIZE] = {5, 4};
    set_free(test_block_2, 8, 0);
    CTEST_ASSERT(memcmp(test_block_2, compare_block_2, BLOCK_SIZE) == 0, "assert set_free() clears first bit of second byte for 0b0000010100000101");
  
    // set test block to 0b00000101
    unsigned char test_block_3[BLOCK_SIZE] = {5};
    unsigned char compare_block_3[BLOCK_SIZE] = {7};
    set_free(test_block_3, 1, 1);
    CTEST_ASSERT(memcmp(test_block_3, compare_block_3, BLOCK_SIZE) == 0, "assert set_free() sets second bit to 1 for 0b00000101");
}

void test_alloc(void) {

    unsigned char read_block[BLOCK_SIZE];
    unsigned char compare_block[BLOCK_SIZE] = {1};
    int alloc_return;

    image_open("test_file", DO_TRUNCATE);

    alloc_return = alloc();
    CTEST_ASSERT(alloc_return == 0, "assert alloc() returns 0 first");
    
    bread(DATA_MAP_NUM, read_block);
    CTEST_ASSERT(memcmp(read_block, compare_block, BLOCK_SIZE) == 0, "assert written memory matches alloc");

    alloc_return = alloc();
    CTEST_ASSERT(alloc_return == 1, "assert alloc() returns 1 second");
    
    alloc_return = alloc();
    CTEST_ASSERT(alloc_return == 2, "assert alloc() returns 2 third");
}

void test_mkfs(void) {

    image_open("test_file", DO_TRUNCATE);
    unsigned char read_block[BLOCK_SIZE];

    mkfs();
   
    int end_of_file = lseek(image_fd, 0, SEEK_END);
    CTEST_ASSERT(end_of_file == 4194304, "assert file size is 4096*1024 = 4194304");
   
    bread(DATA_MAP_NUM, read_block);
    int free_block_num = find_free(read_block);
    CTEST_ASSERT(free_block_num == 7, "assert 6 blocks reserved on making file system");

    image_close();
}

void test_find_incore_free(void) {

    // returns an inode with size 0 on new incore stack
    struct inode test_inode = *find_incore_free();
    // printf("made it here"); 
    CTEST_ASSERT(test_inode.size == 0, "assert find_incore_free() returns empty (size 0) inode on a new incore stack");

    // returns NULL on a full incore stack
    fill_incore_array();
    CTEST_ASSERT(find_incore_free() == NULL, "assert find_incore_free() returns NULL on a full incore stack");

    empty_incore_array();
}

void test_find_incore(void) {

    // returns inode with correct inode num, and non-zero ref_count
    fill_incore_array();
    struct inode test_incore = *find_incore(0);
    CTEST_ASSERT(test_incore.ref_count == 1, "assert find_incore(0) returns full incore after incore array is filled");
   
    // returns NULL if invalid inode num
    empty_incore_array();
    CTEST_ASSERT(find_incore(0) == NULL, "assert find_incore(0) returns NULL if incore array is empty");
}

void test_read_write_inode(void) {

    // set up file system and write to block 3
    image_open("test_file", DO_TRUNCATE);
    mkfs();
    struct inode test_inode = {1, 2, 2, 2, 2, {1}, 1, 3};
    write_inode(&test_inode);

    // create a blank struct to read into
    struct inode test_read_inode = {0}; 
    CTEST_ASSERT(test_read_inode.size == 0, "assert test_read_inode returns empty size before reading");
    
    read_inode(&test_read_inode, 3);

    // CTEST_ASSERT(find_)
    CTEST_ASSERT(test_read_inode.size == 1, "assert read_inode() reads size correctly");
    CTEST_ASSERT(test_read_inode.owner_id == 2, "assert read_inode() reads owner_id correctly");
    CTEST_ASSERT(test_read_inode.permissions == 2, "assert read_inode() reads permissions correctly");
    CTEST_ASSERT(test_read_inode.flags == 2, "assert read_inode() reads flags correctly");
    CTEST_ASSERT(test_read_inode.link_count == 2, "assert read_inode() reads link_count correctly");
    CTEST_ASSERT(test_read_inode.block_ptr[0] == 1, "assert read_inode() reads block_ptr[0] correctly");

    image_close();
}

void test_iget_iput(void) {
    image_open("test_file", DO_TRUNCATE);
    mkfs();

    struct inode test_inode = {1, 2, 4, 4, 4, {0}, 1, 3};

    iput(&test_inode);

    struct inode *test_inode_ptr = iget(3);
    CTEST_ASSERT(test_inode_ptr->size == 1, "assert iget() returns correct size");
    CTEST_ASSERT(test_inode_ptr->owner_id == 2, "assert iget() returns correct owner_id");
    CTEST_ASSERT(test_inode_ptr->permissions == 4, "assert iget() returns correct permissions");
    CTEST_ASSERT(test_inode_ptr->flags == 4, "assert iget() returns correct flags");
    CTEST_ASSERT(test_inode_ptr->link_count == 4, "assert iget() returns correct link_count");

    image_close();
}

void test_ialloc(void) {

    image_open("test_file", DO_TRUNCATE);
    mkfs();
   
    // on empty incore array, expect pointer to 0
    struct inode *test_inode = ialloc();
    CTEST_ASSERT(test_inode->size == 0, "assert ialloc() returns inode with size 0 on new incore array and inode map");
    
    // fill inode metadata, assert NULL 
    unsigned char test_block[BLOCK_SIZE];
    memset(test_block, 255, BLOCK_SIZE);
    bwrite(INODE_MAP_NUM, test_block);
    CTEST_ASSERT(ialloc() == NULL, "assert ialloc() returns NULL values on full inode map");

    // fill incore array, assert null
    fill_incore_array();
    CTEST_ASSERT(ialloc() == NULL, "assert ialloc() returns NULL values on full incore array");
}

int main() {

    CTEST_VERBOSE(1);
    test_image_open();   
    test_image_close();
    test_mkfs();
    test_alloc();
    test_bwrite_and_bread();
    test_find_free();
    test_set_free();
    test_find_incore_free();
    test_find_incore();
    test_read_write_inode();
    test_iget_iput();
    test_ialloc();
    CTEST_RESULTS();
    CTEST_EXIT();

    return 0;
}
