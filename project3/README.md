# Project 2 Producers and Consumers

## Introduction

In this project, we're going to create a number of producer threads and consumer threads. And we're going to use semaphores to coordinate their actions.

The producers will add "events" to a queue, and the consumers will remove them. (In this case, our "events" are just integers.)

When the producers are done, the main thread should signal the consumers to exit.

Each producer is its own thread. Each consumer is its own thread.

All producers will have the same producer run function.

All consumers will have the same consumer run function.

# Example Run

You'll launch the program with four command line arguments:

1. Number of producers
2. Number of consumers
3. How many events each producer will generate
4. How many outstanding (produced but not yet consumed) events there can be at a time.

For example, to launch with 2 producers and 4 consumers, and have each producer make 5 events, with a maximum outstanding event count of 2:

```
./pcseml 2 4 5 2
```

Example output (please read the description for how to generate this):

```
P0: adding event 0
P1: adding event 100
C0: got event 0
P0: adding event 1
C0: got event 100
C1: got event 1
P1: adding event 101
P0: adding event 2
C2: got event 101
C3: got event 2
P0: adding event 3
P1: adding event 102
C0: got event 3
C1: got event 102
P0: adding event 4
P1: adding event 103
P0: exiting
C2: got event 4
C3: got event 103
P1: adding event 104
P1: exiting
C1: got event 104
C2: exiting
C0: exiting
C3: exiting
C1: exiting
```

**Note** that all the producer outputs start with `P` and a number--the producers are numbered serially from `0` up to the number of producers minus one.

The same for consumers, except they begin with `C`.

Each producer will add the number of events specified on the command line.

The event number itself is computed as follows, noting that `producer_number` and `event_number` are zero-based:

```
producer_number * 100 + event_number
```

So if we have 3 producers making 2 events, the event numbers will be:

```
0
1   
100
101
200
201
```
