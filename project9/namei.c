#include "namei.h"
#include "inode.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct inode *namei(char *path) {
    // if the path is /, it returns the root directory's incore inode
    // if the path is /foo, it returns foo's incore inode
    // if the path is /foo/bar, it returns bar's incore inode
    // if the path is invalid (i.e. a component isn't found), it returns NULL
    printf("_path: %s\n", path);
    struct inode *root_dir_inode = iget(ROOT_INODE_NUM);

    return  root_dir_inode;
    // ** NOTE ** : For this first part, simply implement namei() so it just returns 
    // the in-core inode for the root directory, /. The other bits can wait until later.

    // You'll want to use iget() with the root directory's inode number to get this information. 
    // (It's OK to #define ROOT_INODE_NUM 0 and use that.)

    // returns an incore inode with a non-zero ref_count
}