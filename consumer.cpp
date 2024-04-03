#include "./consumer.hpp"
#include "./memoryspace.hpp"

const int SLEEP_TIME = 2;
const int NUMBER_OF_ITEMS_PRODUCED = 10;

int main(int argc, char *argv[]){
    //intitalizes a sharedMemory to be used in the consumer
    struct memorySpace *sharedMemory;

    //opens a shared memory location called shm with read and write permission
    int fd = shm_open("shm", O_RDWR, 0);

    //trims the shm to be only the size that we need.
    ftruncate(fd, sizeof(*sharedMemory));

    //maps the memory in the shared space to the local sharedMemory structure and will update anytiem sharedMemory is changed.
    sharedMemory = (struct memorySpace *)mmap(NULL, sizeof(*sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //Consumes NUMBER_OF_ITEMS_PRODUCED items
    for (size_t i = 0; i < NUMBER_OF_ITEMS_PRODUCED; i++)
    {
        //allows the code to continue if there are resources avalible in the table to be consumed
        sem_wait(&sharedMemory->isNotEmpty);

        //Locks the sharedMemory while operating in the Critical zone.
        sem_wait(&sharedMemory->mutexLock);

        //Consumes item, currently just prints item, could be replaced with whatever is desired
        std::cout << "Consumed: " << sharedMemory->table[sharedMemory->out] << std::endl;

        //increments the out flag.
        sharedMemory->out = (sharedMemory->out + 1) % TABLE_SIZE;

        //leaving the critial zone so relases the lock
        sem_post(&sharedMemory->mutexLock);

        //
        sem_post(&sharedMemory->isNotFull);

        sleep(SLEEP_TIME);
    

    }

    shm_unlink("shmpath");
    return 0;
}