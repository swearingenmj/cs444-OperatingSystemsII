### Solutions to project 10: File System Race Conditions

## Race Condition 1

# Where the race is

```
0  struct inode *iget(int inode_num) {
1     struct inode *incore_node;
2 
3     if (find_incore(inode_num) != NULL) {
4         incore_node = find_incore(inode_num);
5         incore_node->ref_count++;
6         return incore_node;
7     } 
...
21 }    
```

# Example of how a race could result in expected values or situations

This race condition could occur on line 3, where process A enters the condition check and verifies find_incore(inode_num) is NOT null.
Then process A is blocked before finishing the iget() function.
Process B could then run another function, such as empty_incore_array(), meaning the condition find_incore(inode_num) would be NULL.
Then once process A resumes, since it has already passed the condition check, it would try to find_incore(inode_num) to save the value but would result in a program crashing error because the program is expecting to store struct inode* data, not a null value.  

# How to fix the race

To fix this race, we could put a lock around lines 3-7. However, this only provides protection against this specific race condition and there are others lurking in the function iget()...

## Race Condition 2 

# Where the race is

```
0  int directory_make(char *path) {
1
2     char dirname[strlen(path)];
3     get_dirname(path, dirname);
4 
5     char basename[strlen(path)];
6     get_basename(path, basename);
7 
8     struct inode *parent_inode = namei(dirname);
9     struct inode *new_inode = ialloc();
10    int new_data_block_num = alloc();
11
12    unsigned char new_data_block[BLOCK_SIZE] = {0};
13    char *dot = (char *)new_data_block + 2;
14    char *double_dot = (char *)new_data_block + INODE_NUM_SIZE + ENTRY_SIZE;
15
16    write_u16(new_data_block, new_inode->inode_num);
17    write_u16(new_data_block + ENTRY_SIZE, parent_inode->inode_num);
18
19    strcpy(dot, ".");
20    strcpy(double_dot, "..");
21
22    new_inode->flags = FLAG_INIT;
23    new_inode->size = ENTRY_SIZE * 2;
24    new_inode->block_ptr[0] = new_data_block_num;
25
26    bwrite(new_data_block_num, new_data_block);
27
28 	  int block_ptr_index = parent_inode->size / BLOCK_SIZE;
29    int parent_block_num = parent_inode->block_ptr[block_ptr_index];
30    int parent_block_offset = parent_inode->size % BLOCK_SIZE;
31    unsigned char parent_block[BLOCK_SIZE] = {0};
32
33    if (parent_block_offset == 0) {
34        parent_block_num = alloc();
35    } else { 
36        bread(parent_block_num, parent_block);
37    }
38
39    write_u16(parent_block + parent_block_offset, new_inode->inode_num);
40
41    char *new_dir_entry = (char *)parent_block + parent_block_offset + 2;
42    strcpy(new_dir_entry, basename);
43
44    bwrite(parent_block_num, parent_block);
45
46    parent_inode->size += ENTRY_SIZE;
47
48    iput(new_inode);
49    iput(parent_inode);
50
51    return 0;
52 }
```

# Example of how a race could result in expected values or situations

This race condition could occur on line 33, where process A enters the condition check and verifies the parent_block_offset does NOT equal 0.
Then process A is blocked before finishing execution.
Process B could then execute, see that parent_block_offset does NOT equal 0, and read from that parent_block. 
Then once process A begins to execute again, it will read in a full parent_block with the same parent_block_offset value.
When process A then tries to write to the block with a new entry, it will over write the entry from process B which will create a discrepancy between the inode and data blocks.

# How to fix the race

To fix this race condition, you would have to put put a lock around lines 8-49 because within that area the parent block is read, conditionally checked, and written to. 
## Race Condition 3

# Where the race is

```
0  int alloc(void) {
1      unsigned char data_map[BLOCK_SIZE] = {0};
2      
3      bread(DATA_MAP_NUM, data_map);
4      
5      int lowest_free = find_free(data_map);
6      
7      set_free(data_map, lowest_free, SET_UNAVAILABLE);
8      
9      bwrite(DATA_MAP_NUM, data_map);
10     
11     return lowest_free;
12 }
```

# Example of how a race could result in expected values or situations

In the alloc() function, the function initializes the data_map array and calls bread() to read the data from the disk into the data_map. If multiple threads execute alloc() at the same time, they may end up reading the same data_map from disk simultaneously. If they find the same lowest_free they may both attempt to modify the data_map to mark the block as unavailable by calling set_free().  

# How to fix the race

To fix the race condition, we can put a lock around lines 7-9. This will ensure that only one thread can modify the data_map at a time.

## Race Condition 4

# Where the race is

```
0  void iput(struct inode *in) {
1      if (in->ref_count == 0) {
2          return;
3      }
4
5      in->ref_count--;
6    
7      if (in->ref_count == 0) {
8          write_inode(in);
9      }
10 }
```

# Example of how a race could result in expected values or situations

If multiple thread call iput() at the same time on the same inode it's possible for them to enter a critical section after checking the ref_count but before decrementing it. Both threads may end up decrementing the ref_count to - and both threads may attempt to write the inode to disk.   

# How to fix the race

To fix this we could do something like this:

```
0  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
1  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
2
3  void iput(struct inode *in) {
4      pthread_mutex_lock(&lock);           // lock it!
5
6      if (in->ref_count == 0) {
7          pthread_mutex_unlock(&lock);     // unlock it! (if we don't need to do anything)
8          return;
9      }
10
11     in->ref_count--;
12    
13     if (in->ref_count == 0) {
14         write_inode(in);
15         pthread_cond_signal(&cond);      // signal that we're done!
16     } else {
17         pthread_cond_wait(&cond, &lock); // wait for someone to signal that they're done!
1      }
19
20     pthread_mutex_unlock(&lock);         // unlock it!
21  }
```

By using a conditional lock we can avoid holding the mutex for the entire duration of the function, allowing other threads to access iput() concurrently.
