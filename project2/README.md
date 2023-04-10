# Project 2 Mutexes

## Introduction

We're going to write a program that allows brokers to reserve (and unreserve) seats at a venue.

The brokers will be simulated and will be reserving and unreserving seats as fast as they can.

Since the reservation status of all the seats is shared between brokers, we have ample opportunities for race conditions.

You want to write functions to reserve and unreserve seats in such a way that the operations are successful no matter how many brokers are running.

