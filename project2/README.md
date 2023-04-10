# Project 2 Mutexes

## Introduction

We're going to write a program that allows brokers to reserve (and unreserve) seats at a venue.

The brokers will be simulated and will be reserving and unreserving seats as fast as they can.

Since the reservation status of all the seats is shared between brokers, we have ample opportunities for race conditions.

You want to write functions to reserve and unreserve seats in such a way that the operations are successful no matter how many brokers are running.

# Example Run

On the command line, we'll be able to specify the number of seats in this venue, the number of brokers who are competing for seats, and the number of transactions each broker will make over the course of the run.

(A transaction is either "reserve a particular seat", or "unreserve a particular seat".)

Running the command without any argument shows help:

```
% ./reservations
usage: reservations seat_count broker_count xaction_count
```

Run it by specifying the seat count, the broker count, and the transaction count in that order.

> **WARNING!** Each broker is its own thread! It's possible to bring your system to its knees by specifying large numbers on the command line. Start small.

Here's a successful run of 10 seats, 5 brokers, and 100 transactions:

```
% ./reservations 10 5 100
Broker 0: That all seemed to work very well.
Broker 2: That all seemed to work very well.
Broker 3: That all seemed to work very well.
Broker 4: That all seemed to work very well.
Broker 1: That all seemed to work very well.
```

However, if your program has data races, you'll see something more like this unsuccessful run:

```
% ./reservations 10 5 100
Broker 0: That all seemed to work very well.
Broker 4: the seat count seems to be off! I quit!
Broker 2: the seat count seems to be off! I quit!
Broker 1: the seat count seems to be off! I quit!
Broker 3: the seat count seems to be off! I quit!
```
