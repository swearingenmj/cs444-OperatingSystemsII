#include "namei.h"
#include "inode.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct inode *namei(char *path) {
    struct inode *root_dir_inode = iget(ROOT_INODE_NUM);
    return  root_dir_inode;
}