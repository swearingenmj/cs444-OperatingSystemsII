# OS II Class Notes


# Exploration 7: fsck and Journaling

Describe a scenario in which the metadata of a disk would be inconsistent. What data would be in disagreement?

A scenario where metadata may be inconsistent is when a system experiences an unexpected power outage while writing a new file to the disk. The file's metadata might reflect that the file exists, but the corresponding data blocks may not have been written fully, leading to a mismatch. 

Describe one type of correction to broken metadata fsck would be able to handle and how it would accomplish it.

One type of correction fsck can make is that it can identify discrepancies and reconcile them by updating the metadata to reflect the correct state of the filesystem. It does this by examining the file systems structures, validating relationships between componenets and repairing any inconsistencies. 

In metadata-only journaling, why does the data block get written before the journal entry is processed?

The data block is written before the journal entry is processed in case of a crash. By writing to disk before processing the journal entry ensures that the data is persistent even if the metadata chances haven't been processed yet. Also, in the event of a crash the journal can be used during the system boot to replay teh entries and bring metadata up to date based on recorded changes.



# A Tiny Bit About Directories

- Maps file names to inode numbers

- In a directory entry: 
    - a filename
    - an inode number

- Every directory has two files to start: `.` and `..`

    Root directory: `/`, inode 0

    filename   inode number
    --------   ------------
       .            0
       ..           0
       foo          1376    (this is a subdirectory)

    Subdirectory: `/foo`, inode 1376

    filename   inode number
    --------   ------------
       .            1376
       ..           0

    cd /foo/../foo/../foo

# What can go wrong during a crash?

- Imagine we're creating a new file...
1. What are all the steps in creating a new file?
    i. Which blocks are touched?
        - free inode block
        - inode data block (inode record for this file)
        - inode data block (inode record for the directory)
        - free map block
            - if the directory overflows
            - if we were writing to the new file
        - block containing the directory entry
    ii. What happens in sequence? ^^^
        - get a free inode block for the new file
        - find the inode for the directory the file will go in
        - check if the directory block is full, if full:
            - get a new free block fo the directory
            - update directory inode (new block ptr, new size)
        - add to the directory

2. What can go wrong if the process is interrupted at any of these steps? 
    i. Power outage, computer freezes, etc.
        - When getting a new free block, if there is a crash the block will be marked as used but is not actually being used because of a crash! "Phantom memory use" tmtmtmtm
        - When the block is declared used, then a crash happens, there is nothing written to the block even though nothing has been written to it. 
        - 

# A Small Journaling Primer

1. Write to disk what we're about to store on disk.
2. Write to disk the data we want to write.
3. Delete the stuff from step 1. 

# Journaling

## How it works

- Let's write in the 'journal' what we're about to do.
- Then let's do those things.
- Then let's remove the entry from the journal.

Example: 
    - Create a journal entry:
        - Get an inode for the file
        - Add a directory entry for the file
        - Save the inode and directory entry
    - _Checkpoint_ the journal - take what's in the journal entry and do it!
        - Do all the things in the journal entry
    - Remove the journal entry

    - The journal entry:

    ```
    |Transaction start|Action|Action|...|Transaction end|
    ```

What happens then the power goes out:
- Partway through the journal entry
- Between the journal write and the disk write
- Partway through the disk write
- Between the disk write and removing the journal entry
- Partway through removing the journal entry

How do you recover?

## Group exploration: understand how it can recover from a crash

## Metadata-only journaling

```
Write block to disk
|Ts|Alloc|Update inode|Te|
```

## Brain primer: directories
- Directories are files that map filenames to inode numbers
- They have a bunch of "entries" that do this on a per-file basis.

## Group brainstorm: more project questions!


