#ifndef DIR_H
#define DIR_H

struct directory *directory_open(int inode_num);
int directory_get(struct directory *dir, struct directory_entry *ent);
void directory_close(struct directory *d);

#endif