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



Why does having locks on both the head and tail of the concurrent queue have a performance advantage over having one big lock for the whole thing?

Why doesn't the concurrent hash table in the reading need any locks in any of the Hash_ functions?

In the concurrent queue in the book, speculate on why the dummy node is required.


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

