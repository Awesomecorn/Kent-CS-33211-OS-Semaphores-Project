#ifndef MEMORYSPACE_HPP
#define  MEMORYSPACE_HPP

#include <sys/mman.h>
#include <sys/stat.h>     
#include <fcntl.h>  
#include <semaphore.h>


#define TABLE_SIZE 2
const char *SHMPATH = "shmfile";

struct memorySpace {
    int table[TABLE_SIZE];

    int in;
    int out;

    sem_t mutexLock;
    sem_t isNotFull;
    sem_t isNotEmpty;
};

#endif // MEMORYSPACE_HPP