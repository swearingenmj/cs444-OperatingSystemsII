# OS II Class Notes

# Exploration 1.1: Multithreading

What are the main differences between threads and processes?

A process is a task that is being completed by the computer. A thread is similar to a process but, unlike processes, threads can share data. 

What does a thread do when it tries to join but the other thread hasn't completed yet?

When a thread tries to join but the other thread hasn't completed yet the thread trying to join will be blocked and will sleep until the other thread finishes. 

What's the difference between concurrency and parallelism?

Concurrency is when a program can produce correct results even if there are multiple threads running at the same time. Parallelism is when a program can run multiple threads at the same time. 

Global variables are shared among threads, but local variables are not. Describe why this is the case.

Each thread will have its own copy of a local variable because threads maintain their own stack. Global variables are shared among threads because they are stored in the heap.

# Exploration 1.2: The pthreads API

What's the function to create a new thread?

pthread_create()

What's the function to wait for a thread to finish?

pthread_join()

How do you pass an argument to the new thread?

You can pass an argument to the new thread by passing a pointer to the argument as the fourth argument to pthread_create().

How would you pass multiple arguments to the new thread?

You can pass multiple arguments to the new thread by passing a pointer to a struct that contains all the arguments as the fourth argument to pthread_create().


# Threads
- A process can spawn many threads
- Threads share gloabal data
    - ... But not locals!

# Parallelism
- Avoiding blocking on I/O

# Thread operations
- Create
    - `pthread_create()`

    ```
    void *run(void *arg) {
        char *string_to_print = arg;   // you can assign void pointer to be a char pointer to print it easier! 

        // run thread stuff!
        printf("%s\n", (char *)arg);   // this is a void pointer, so we need to cast it to a char pointer OR we can do the above assignment of arg to string_to_print
        printf("%s\n", string_to_print); 
    }
    int main(void) {
        pthread_t t1, t2;
        int x = 12;

        pthread_create(&t1, NULL, run, "hi there");
        pthread_create(&t2, NULL, run, x);
    }
    ```

- Join -- wait for a thread to finish
    - `pthread_join()`
- Kill
    - `pthread_kill()`


# More notes on pthread
#include <pthread.h>

int
pthread_create(pthread_t *thread,    // this is our identifier
               const pthread_attr_t *attr,    // we're going to replace this will null!
               void *(*start_routine) (void *),   // start_routine is a pointer to a function and accepts a void pointer as an argument
               void *arg);    // lets us pass a pointer to a thing

# Concurrency
- Have a program run correctly when multiple threads are using shared resources.

# Race Conditions
- When the results  of a computation depends on the order in which the threads execute the instructions. 

# Critical Sections
- A section of code that can cause a race condition. 

# Race Demo

    Withdrawing money from an ATM

```
balance: $150
amount: $100

function withdraw(amount):  <- Process 1 is here and will dispense $100 because balance is $150
    if (amount <= balance): 
        balance = balance - amount   <- Process 2 is here up is paused/sleeping... Process 2 will dispense $100 because the "balance is supposedly $150" ... However the balance is actually $50 because Process 1 dispensed $100
        dispense(amount)
    else:
        return "Insufficient funds"
```

    After both process 1 and process 2 run, the balance of the account will be $-50! This is a concurrency issue!
    How do we make sure only one thread executes at a time? Put a lock around it!


# Locks/Mutexes Intro

```
balance: $150
amount: $100
locked?: yes

function withdraw(amount):  
    lock()      <- Process 1 cannot move on because Process 2 has locked the function! It will sleep until Process 2 is complete.

    if (amount <= balance): 
        balance = balance - amount   <- Process 2 has locked the function and will complete the withdraw before unlocking the program and allowing Process 1 to run
        dispense(amount)
        return_value = OK
    else:
        return_value = INSUFFICIENT_FUNDS

    unlock()    <- Process 2 will unlock the function and allow Process 1 to run

    return return_value
```
      

