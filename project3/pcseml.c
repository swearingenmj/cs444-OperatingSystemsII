#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "eventbuf.h"

// Initialize command line args
int number_of_producers;
int number_of_consumers;
int number_of_events;
int outstanding_events;

// Synchronization semaphores
sem_t *empty;
sem_t *full;
sem_t *mutex;

// Initialize event buffer
struct eventbuf *eventbuf;

// Create a semaphore
sem_t *sem_open_temp(const char *name, int value) {
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

// Producer run function

void *producer(void *arg) {
    int *id = arg;

    for (int i = 0; i < number_of_events; i++) {
        int event_number = *id * 100 + i;

        // Wait to see if there is space in the event buffer
        sem_wait(empty);

        // Lock mutex around the event buffer
        sem_wait(mutex);

        // Print that it's adding the event, along with the event number ex. "P0: adding event 0"
        printf("P%d: adding event %d\n", *id, event_number);

        // Add an event to the eventbuf ex. "P1: adding event 100"
        eventbuf_add(eventbuf, event_number);

        // Unlock the mutex
        sem_post(mutex);

        // Signal waiting consumer threads that there is an event to be consumed.
        sem_post(full);
    }
    // "P1: exiting"
    printf("P%d: exiting\n", *id);

    return NULL;
}

void *consumer(void *arg) {
    int *id = arg;
    
    while (1) {
        // Wait to see if a producer has put anything to buffer
        sem_wait(full);

        // Lock a mutex around the eventbuf
        sem_wait(mutex);

        // Check to see if the eventbuf is empty
        if (eventbuf_empty(eventbuf)) {
            // if it is, we're done! time to unlock the mutex and exit the loop
            sem_post(mutex);
            sem_post(full);
            printf("C%d: exiting\n", *id);
            return NULL;
        }
        // If the eventbuf is not empty, get the event from it
        int event_number = eventbuf_get(eventbuf);

        // Print the message about the event being received ex. "C0: got event 0"
        printf("C%d: got event %d\n", *id, event_number);

        // Unlock the mutex
        sem_post(mutex);

        // If we're not done, post to the semaphore indicating that there are not free spaces for producers to put those events into
        sem_post(empty);
    }
    // "C0: exiting"
    printf("C%d: exiting\n", *id);

    return NULL;
}

int main(int argc, char *argv[]) {
    // Parse the command line
    if (argc != 5) {
        printf("Usage: ./pcseml <number of producers> <number of consumers> <number of events> <outstanding events>");
        return 1;
    }

    number_of_producers = atoi(argv[1]);
    number_of_consumers = atoi(argv[2]);
    number_of_events = atoi(argv[3]);
    outstanding_events = atoi(argv[4]);

    // Create the event buffer
    eventbuf = eventbuf_create();

    // Create the semaphores
    empty = sem_open_temp("empty", outstanding_events);
    full = sem_open_temp("full", 0);
    mutex = sem_open_temp("mutex", 1);

    // Start with the correct number of producer threads
        // Each thread will be passed a pointer to an int (its id)
    pthread_t producer_threads[number_of_producers];
    int producer_ids[number_of_producers];

    for (int i = 0; i < number_of_producers; i++) {
        producer_ids[i] = i;
        pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
    }

    // Start with the correct number of consumer threads
        // Each thread will be passed a pointer to an int (its id)
    pthread_t consumer_threads[number_of_consumers];
    int consumer_ids[number_of_consumers];

    for (int i = 0; i < number_of_consumers; i++) {    
        consumer_ids[i] = i;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_ids[i]);
    }

    // Wait for all the producer threads to complete
    for (int i = 0; i < number_of_producers; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    // Notify all the consumer thread that they are done
        // This should cause them to exit
    sem_post(full);

    // Wait for all consumer threads to complete
    for (int i = 0; i < number_of_consumers; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Free the event buffer
    eventbuf_free(eventbuf);
}