# Project 6: Adding inode Support 

## Introduction

Now that we have the basics of the file system implemented, we're going to add more code to implement support for inodes. If you recall, an inode stores metadata about a file, like size, permissions, etc. Blocks 3-6 hold the array of inodes.

This week's goals are to:
- Learn about reading and writing multibyte values from a buffer
- Learn how inode data is stored on disk.
- Find an inode on disk by number.
- Learn about in-core inodes.
- Write functions to find free in-core inodes and find in-core inodes by number.
- Write code to load and save inode data from and to disk.
- Write code to retrieve and free in-core inodes.
- Modify ialloc() to initialize and return an in-core inode.

It is strongly suggested you read this entire file in order one time, then go through it again in order during implementation.

You can test as you go, making your life easier, or you can test after you've written it all if you want the extra challenge.
