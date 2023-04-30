# Project 4: Finding Races and Deadlocks

Submit a written description of where the races and deadlocks are, how they occur,  and how to fix them (i.e. where to add locks). Feel free to include the code with arrows or however you think is the most clear.

## Part 1: Race Conditions

In the following pseudocode examples, x and hash are variables shared between threads. The other variables are not shared.

lock() is a mutex lock, and unlock() is a mutex unlock. m1 is a shared mutex variable.

For each of these, write a short description of how the race occurs and what could be done to fix it.

(There might be more than one race in an example!)

1. > ```
   > if x == 12:
   >     x++;
   > ```

If two or more threads enter the if statement at the same time this will create a race condition because both may read x as 12 and then try to increment it. To fix this we can use a mutex before checking the value of x.

2. > ```
   > if x == 12:
   >     lock(m1);
   >     x++;
   >     unlock(m1);
   > ```

This uses a mutex to protect the critical section, but could result in a race condition if another thread modifies x outside of the critical section causing x to be a value other than what is expected. To fix this we could acquire the same mutex lock before checking the value of x. 

3. > ```
   > if y not in hash:
   >     hash[y] = 12
   > else:
   >     hash[y]++
   > ```

This code has a race condition where two threads could try to update the hash map at the same time. If y is not in hash for both thread they could both try to add it with the value 12. This would result in only one update instead of two. Also, if y is already in hash for both threads, they could both try to increment the value at the same time resulting in one increment instead of two. To fix this we could acquire the mutex before checking if y is in the hash map and release the mutex after updating or incrementing the value of y.

4. > ```
   > x += 12;
   > ```

In this code x is shared between threads making it possible for multiple threads to access and modify the value of x simultaneously. To fix this we could acquire the mutex before modifying the value of x and release the mutex after modifying it.

5. > ```
   > x = 0
   >
   > semaphore_init(value):
   >     x = value
   >
   > semaphore_signal():
   >     x++;
   >
   > semaphore_wait():
   >     while x == 0:
   >         do nothing    # spinlock
   >
   >     x--
   > ```

In this code it could be possible for multiple thread to execute semaphore_signal() or semaphore_wait() at the same time. This could result in x being incremented or decremented more than once. To fix this we we could use a mutex lock to protect access to the variable x inside the semaphore functions. Specifically, we should acquire the mutex lock before modifying or checking the value of x, and release the mutex lock after the modification or check is complete. Alternatively, we could use atomic operations to increment and decrement x instead of a mutex lock, which would avoid the need for a lock and provide thread-safe updates to x.


## Part 2: Deadlocks

Multiple threads are running through the following code examples. For each, describe how deadlock can occur and what can be done to fix it.

1. > ```
   > function1():
   >     lock(m1)
   >     lock(m2)
   >
   >     unlock(m2)
   >     unlock(m1)
   >
   > function2():
   >     lock(m2)
   >     lock(m1)
   >
   >     unlock(m1)
   >     unlock(m2)
   > ```

A deadlock may occur if function1() acquires the lock on m1 and function2() acquires the lock on m2 at the same time. Both functions will be waiting for other lock to be released before they can continue. To fic this we can ensure that both functions acquire the locks in the same order. For example, we could have both functions acquire the lock on m1 before acquiring the lock on m2.

2. > ```
   > function1():
   >     lock(m1)
   >     lock(m2)
   >
   >     unlock(m2)
   >     unlock(m1)
   >
   > function2():
   >     lock(m1)
   >     lock(m2)
   >
   >     unlock(m1)
   >     lock(m1)
   >
   >     unlock(m2)
   >     unlock(m1)
   > ```

If function1() acquires the lock on m1 and function2() acquires the lock on m2 at the same time a deadlock might occur because function2() will try to acquire the lock on m1 while holding the lock on m2. This could cause a deadlock because function1() is waiting for the lock on m2, which is held by function2(), while function2() is waiting on the lock on m1 which is held by function1(). To fux this we can modify function2() to acquire the lock on m1 before acquiring the lock on m2. We could also use a single mutex that both functions acquire in the same order. 

3. > ```
   > function(m1, m2):   # Mutexes are passed in by caller
   >     lock(m1)
   >     lock(m2)
   >
   >     unlock(m2)
   >     unlock(m1)
   > ```

If two threads simultaneously call the function and each thread acquires one mutex before the other this could create a deadlock. To prevent this we can ensure that all threads acqure the mutexes in the same order. For example, we could have all threads acquire the lock on m1 before acquiring the lock on m2.
