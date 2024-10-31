-- Operating Systems, Coding Assignment 1, KSU --
- By Will Allen -
- Last Updated : 10/28/24 - 

This program includes a producer and consumer (written as separate files)
Semaphores with a shared buffer are used to access the critical section
and to solve the Producer-Consumer Problem

EXECUTING THE PROGRAM - 
Enter these three lines in program terminal consecutively. 
Linux/Unix system required.

$ gcc producer.c -pthread -lrt -o producer
$ gcc consumer.c -pthread -lrt -o consumer
$ ./producer & ./consumer &