#ifndef MEMORYSPACE_HPP
#define  MEMORYSPACE_HPP

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>  
#include <semaphore.h>


const int TABLE_SIZE = 2;
const char *SHMPATH = "/SHM";

struct memorySpace {
    int table[TABLE_SIZE];

    int in;
    int out;

    sem_t sem;
    sem_t empty;
    sem_t full;
};

#endif // MEMORYSPACE_HPP