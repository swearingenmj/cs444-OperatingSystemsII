# OS II Class Notes


# Exploration 2.1: Locks

Why should locks be held for as short a time as possible?

A lock should be held for as short a time as possible because this will avoid the overhead of OS process rescheduling. 

With a reader/writer lock, why can the single writing thread be the only thread in the critical section?

Only a single writing thread can be in the critical section because we don't want multiple threads writing and changing information simultaneously.

What's a recursive mutex useful for?

A recursive mutex is used to protect shared data from being simultaneously accessed by multiple threads.

Looking at the book, explain in human terms the operation of Test-and-Set.

The test_and_set operation works by setting a lock to 1 and returning the previous value of the lock. If the lock was previously set to 1, the thread will spin until the lock is set to 0.

Looking at the book, explain in human terms how Test-and-Set can be used to create a spin lock.

In the book and in the lecture, Test-and-Set is used to create a spin lock. The threads are forced to wait at a while loop that checks the test_and_set function to see if the lock is locked. If the lock is locked, the thread will continue to cycle through the while loop until the lock is unlocked. Once it is unlocked the thread can then acquire the lock with the test_and_set function. Also, because of CPU magic the test_and_set function is atomic, so the thread will not be interrupted while it is checking the lock.


# Exploration 2.2: Locking Data Structures

What are the tradeoffs with the Approximate Counter in the book when updating the global counter more or less frequently?

The trade off with the approximate counter is between accuracy and performance. If the global counter is updated more frequently, the accuracy of the counter will be higher, but the performance will be lower. If the global counter is updated less frequently, the accuracy of the counter will be lower, but the performance will be higher.

Why does having locks on both the head and tail of the concurrent queue have a performance advantage over having one big lock for the whole thing?

Having locks at the head and the tail of the concurrent queue allows for concurrency between enqueues and dequeues. If there was only one lock for the whole queue, then the queue would be locked while an enqueue or dequeue was happening, and the other operation would have to wait until the lock was released.

Why doesn't the concurrent hash table in the reading need any locks in any of the Hash_ functions?

The concurrent hash table uses a lock per hash bucket which is represented by a list. The Hash_ functions only access one bucket at a time, so there is no need for a lock on the whole table.

In the concurrent queue in the book, speculate on why the dummy node is required.

I think the dummy node is required to help you maintain the head and tail pointers. By using the dummy node you can ensure that the tail doesn't fall behind the head and that it will never point to a node that is not accessible through the head pointer. I also think it keeps the head and tail from being set to null. 



# Locking Things

```
int lock = 0;

void lock(void) 
{
    /* 
    while (lock == 1) 
    {
        // spin
    }
    lock = 1;
    */

    while (test_and_set(&lock, 1) == 1) 
    {
        // spin
        sleep_until_lock_is_released();
    }
}

void unlock(void) 
{
    lock = 0;
}
```

# Test and Set ("Atomic Exchange")

```
int test_and_set(int *old_ptr, int new_val)
{
    // this can't be interrupted between here vvvvv
    int old_val = *old_ptr;

    *old_ptr = new_val;
    // and here ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    return old_val;
}
```

# pthread Locks ("mutexes")

```
int lock = 0;

void lock(void) 
void unlock(void) 

---

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&lock);
pthread_mutex_unlock(&lock);

```

