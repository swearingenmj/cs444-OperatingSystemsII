# OS II Class Notes

# Exploration 3.1: Condition Variables

In the sample code above, comment out the sleep(1); line and run it. What happens? Why does it happen?

When we comment out the sleep(1) line the program doesn't wait for the thread to finish before it prints the message. This is because the Waiter thread is not waiting for the program to increment to 5 before it prints the message. The Waiter thread is just printing the message and then the program finishes.

What's the difference between a condition variable and the condition itself (the condition data)?

The condition is the logical expression that evaluates to true or false. The condition variable is the object that allows threads to wait until the condition is true.

Why should the signaling thread hold the mutex before doing so?

The signaling thread should hold the mutex to ensure that the state of the shared data is consistent. If the signaling thread didn't hold the mutex another thread may modify the shared data before the waiting thread had a chance to wake up and acquire the mutex. 

Why is while-loop a good idea for threads waiting on a condition variable?

A while loop ensures that the thread checks the condition after being woken up, and if the condition is not yet true, the thread will go back to sleep and wait for the condition variable to be signaled again. 

If the waiting thread has acquired the mutex and is waiting on a call to pthread_cond_wait(), how can the signaling thread acquire the mutex seemingly at the same time?

The signaling thread must hold the mutex when it modifies the shared data and signals the condition variable to ensure that the state of the shared data is consistent. However, it releases the mutex before calling pthread_cond_wait() to allow other threads to modify the shared data while the waiting thread is blocked. This allows for concurrent access to the shared data while still ensuring data consistency and preventing race conditions. 

Why are two mutexes (one for the producer threads and one for the consumer threads) required for a good producer/consumer implementation?

Two mutexes are used to synchronize access to the shared data. One mutex is used to protect the buffer itself and the other mutex is used to ensure the producer and consumer threads don't access the buffer at the same time.

# Exploration 3.2: Semaphores

Why would you set a semaphore to a value greater than 1? What would that represent?

Setting a sempaphore to a value greater than 1 represents the number of threads that can access the shared data at the same time. This can be used to allow multiple threads to access the shared data at the same time, or to allow multiple threads to access the shared data in a specific order.

With reader/writer locks, why is it OK to have a lot of readers accessing a critical section simultaneously? Why don't they step on each other's toes?

It is OK to have a lot of readers accessing a critical section simultaneously because the readers are not modifying the data and there is no risk of data inconsistencies. There is no need for readers to block or wait for each other to finish reading the data which allows for better concurrency and can improve performance.

# Conditional Variables
## Wait, Signal, and Broadcast

# Semaphore 
## They maintain a count

#### Operations
- Wait
    - If you wait:
        - Decrement the count
        - Keep on running
    - Else:
        - Sleep

- Post, Signal
    - If you signal/post:
        - Increment the semaphore count
            - Wake up a waiting thread
        - Keep on running

# Use Cases

- Can be like a mutex
    - Set the semaphore value to 1
    - A "wait" acts like a "lock"
    - A "signal" acts like an "unlock"

- Force the order of threads
    - Set the semaphore value to 0
    - Have the thread to do 2nd wait on the semaphore
    - Let the thread to do 1st run..
    - Then the first thread can signal when ready

- Allowing _n_ threads to do a thing at once
    - Set the semaphore count to _n_
    - Set count to 10
    - Run producer
        - loop
            - wait
            - add item to queue
    - Run consumer
        - loop
            - remove item from queue
            - post

# Deadlock



 