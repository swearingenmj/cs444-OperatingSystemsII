# OS II Class Notes


# Exploration 8: SSDs

What could go wrong if SSDs used a direct mapping from logical blocks to pages?

If SSDs used a direct mapping from logical blocks to pages a few different things could go wrong. First, there may be uneven wear leveling because some pages would experience more write/erase cycles than others. Additionally, there could be poor performance because a page needs to be erased before it can be written to.

What is it about log structuring that causes garbage collection to be required?

Log structuring requires garbage collection because when new data is written it is appended to the end of a log segment. When the data is updated the original data remains on the log segment but it's marked as stale. The log segment then becomes fragmented with a mix of valid and stale data. Garbage collection is required to reclaim the space occupied by stale data.

What is write amplification?

Write amplification is used to measure the efficiency of write operations in SSDs. Its the ratio of the amount data written by the host system and the actual amoutn of data written to the underlying store media. 

## Directories

- Purpose: map a filename to its inode number
- Directories are files -- just a *special* type of file
    - They have an inode
    - The data associated with the directory is a bunch of *directory entries*
        - This data is stored in regular data blocks.
- Directory Entry:
    - 2 bytes: inode number
    - 16 bytes: filename
    - 14 bytes: reserved (nothing-ness)!
- `size` of the Directroy (in its inode)
    - Number of entries * size of each entry

## Root Directory

- Special in that it's created automatically -- by `mkfs()`!
- Will have inode #0
    - Normally the root inode number would be stored in the superblock!
- There are two directory entries in every directory:
    - `.` `..`
    - The current directory, parent directory
- What do the first two directory entries for the root directory contain? 

inode     filename
-----     --------
  0          .
  0          ..
  1          foo.txt

  create /foo.txt
  open /foo.txt


## Group discussion: What needs to happen in order to create the root directory?

### What needs to be called/allocated/written to/etc.?

1. Get the inode for the directory (`ialloc()`).
2. Get a block to hold directory entries (`alloc()`).
    - Other metadata:
        - `size`: set to 64 (2 entries * 32 bytes) 
        - `link_count`: set to 1
        - `flags`: set to 2 (directory type)
3. Write the block number to the inode `block_ptr` array.
    - Have the inode point to the data block
4. In the block, set up the `.` and `..` entries.
    - Assign current and parent to point to root.
5. `iput()` the root in-core inode, write to disk.
6. `bwrite()` the data block with the directory 


```
void ls(void) {
    struct directory *dir;
    struct directory_entry ent;

    dir = directory_open(0);

    while (directory_get(dir, &ent) != -1) {
        printf("%d %s\n", ent.inode_num, ent.name);
    }

    directory_close(dir);
}
```
