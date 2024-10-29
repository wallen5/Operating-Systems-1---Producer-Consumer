#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

void PrintSemaphoreValue(std::string ident, sem_t *semaphore, int &value);
int main()
{
    const char *ident = "Shared Memory";
    const char *fillSem = "Full";
    const char *avail = "Available"; //for semaphores open functions
    const char *mutexSem = "Mutex";
    const int SIZE = 2;
    sem_t *fill, *ready, *mutex;
    int shared_memory_file_descriptor;
    int *buffer;
    int loop_count = 5;

    // Make shared process between prod and cons
    shared_memory_file_descriptor = shm_open(ident, O_CREAT | O_RDWR, 0666);

    // Set the size of the shared memory
    ftruncate(shared_memory_file_descriptor, sizeof(int));

    // Map the shared memory onto the address space
    buffer = (int *)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_file_descriptor, 0);

    fill = sem_open(fillSem, O_RDWR);

    ready = sem_open(avail, O_RDWR);

    mutex = sem_open(mutexSem, O_RDWR);

    int value = -1;
    while(loop_count--){
        std::cout<<"in con\n";
        sem_wait(fill);
        sem_wait(mutex);                   // lock the critical section

        (* buffer)--;   
        sem_post(mutex);                   // release the lock on critical section
        std::cout << "consumed an item\n"; //take item out of buffer
        sem_post(ready); //send that semephore is open
    }
    
    std::cout << "limit " << *buffer << " left\n";
    
    
    // Close and unlink semaphores Remove Shared memory
    sem_close(fill);
    sem_close(ready);
    sem_close(mutex);
    sem_unlink(fillSem); // close all opened semaphores
    sem_unlink(avail);
    sem_unlink(mutexSem);

    /* close and unlink shared memory*/
    munmap(buffer, sizeof(int));
    close(shared_memory_file_descriptor);
    shm_unlink(ident);

    return 0;
}



void PrintSemaphoreValue(std::string ident, sem_t *semaphore, int &value) {
    std::cout << ident << " value: " << sem_getvalue(semaphore, &value) << std::endl;
}