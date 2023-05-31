#include "ls.h"
#include "dir.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

// TODO: test code
void ls(void){
    printf("_ls\n");
    struct directory *dir;
    struct directory_entry ent;

    dir = directory_open(0);

    while (directory_get(dir, &ent) != -1)
        printf("%d %s\n", ent.inode_num, ent.name);

    directory_close(dir);
}