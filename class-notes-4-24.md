# OS II Class Notes


# Exploration 4.1: Concurrency Bugs

What are a simple sequence of instructions that can cause a deadlock?

A simple sequence of instructions that can cause a deadlock is when two threads are waiting for each other to finish. For example, if thread 1 acquires mutex 1 and continues up to the point where mutex 2 needs to be acquired. Thread 2 acquires mutex 2 and continues up to the point where mutex 1 needs to be acquired. Both threads are now waiting for the other thread to release the mutex they need to continue.

What is livelock?

Livelock is when two threads are trying to acquire the same resource and are constantly retrying to acquire the resource. This is different from deadlock because the threads are not blocked, they are just constantly retrying to acquire the resource.

What is the difference between partial ordering and total ordering?

Partial ordering allows for some concurrency and parallelism whereas total ordering enforces strict sequencing of all operations.

What are more common, deadlock bugs or non-deadlock bugs?

Non-deadlock bugs are more common than deadlock bugs.

Describe step-by-step how condition variables can be used to prevent order violations.

To use a condition variable to prevent order violations we can add a condition variable, corresponding lock, and a state variable. When the code runs it will set the state variable to 1 and signal that it has done that before releasing the lock. The other thread will then acquire the lock and check the state variable. If the state variable is 1 it will continue, otherwise it will wait for the condition variable to be signaled. This ensures that the threads will run in the correct order. 

What are some methods of deadlock avoidance?

Some methods of deadlock avoidance are:
- Acquire locks in a consistent order
- Use a timeout when acquiring locks
- Use a deadlock detection algorithm


# Exploration 4.2: Event-Based Concurrency

What does "blocking" or "to block" mean?

Blocking means that a thread is waiting for an event to occur before it can continue.

In what ways is using event-based concurrency easier than thread-based?

Event-based concurrency is easier than thread-based because there is only one event handled at a time and there is no need for locks. 

In what ways is using thread-based concurrency easier than event-based?

Thread-based concurrency is easier than event-based when a system call might block for a long time. In this case, event-based concurrency would block the entire program until the system call returns. Thread-based concurrency would allow other threads to continue running while the system call is blocking.

What problem does select() solve?

select() allows you to check the description to see if the incoming I/O should be processed immediately or if it should be processed later.

Does a standard event loop use multiple CPU cores? If so, how? If not, what do you have to utilize those additional cores with event-based concurrency?

A standard event loop does not use multiple cores. To utilize the additional cores you can have the cores handle separate events or use worker threads to handle tasks with the main event loop. 
