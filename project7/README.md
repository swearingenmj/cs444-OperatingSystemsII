# Project 7: Implementing Directories to the File System

## Introduction

**Remember:**
- Understand this spec completely before writing a line of code. DM questions early and often.
- Don't hardcode any magic numbers!

So far we've just been building the lower-level building blocks of the file system. Now, at last, it's time to construct something tangible.

We're going to add the root directory to the file system! And we're going to create functions to access those directory entries.

The big picture:
1. Create the root directory--this will be the job of mkfs().
2. Write a function to "open" the directory so you can read the entries.
3. Write a function that will read directory entries.
4. Write a function that will close the directory.
5. Use those functions to write a function called ls() that will show the entries in the root directory.

In a later project, we'll implement an algorithm to find the inode of an item at the end of a path, like the inode for frobozz on the path /foo/bar/baz/frobozz.
