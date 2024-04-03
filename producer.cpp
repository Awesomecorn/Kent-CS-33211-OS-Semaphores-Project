#include "./producer.hpp"
#include "./memoryspace.hpp"

const int SLEEP_TIME = 1;
const int NUMBER_OF_ITEMS_PRODUCED = 10;

int main(int argc, char *argv[]){
    //intitalizes a sharedMemory to be used in the consumer
    struct memorySpace *sharedMemory;

    //opens a shared memory location called shm with read and write permission. Verifies that there is not yet a shared memeory location with this name.
    int fd = shm_open("shm", O_CREAT | O_EXCL |O_RDWR, 0600);

    //trims the shm to be only the size that we need.
    ftruncate(fd, sizeof(*sharedMemory));

    //maps the memory in the shared space to the local sharedMemory structure and will update anytiem sharedMemory is changed.
    sharedMemory = (struct memorySpace *)(mmap(NULL, sizeof(*sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    //Initializes mutexLock to be open because nothing is currently in the critical section.
    sem_init(&(sharedMemory->mutexLock), 1,1);
    
    //isNotEmpty will allow things past when the table is not Empty. Incremented every time producer produces. And intitalized to 0 because the table starts as empty.
    sem_init(&(sharedMemory->isNotEmpty), 1,0);
    
    //isNotFull will allow things past when the tabel is not Full. Incremented every time the consumer consumes. And intitalize to 2 because the table starts as empty.
    sem_init(&(sharedMemory->isNotFull), 1, TABLE_SIZE);

    sharedMemory->in = 0;
    sharedMemory->out = 0;

    //Produces NUMBER_OF_ITEMS_PRODUCED items
    for (size_t i = 0; i < NUMBER_OF_ITEMS_PRODUCED; i++)
    {
        //to be useful randomValue would be replaced with whatever needs to be produced.
        int randomValue = i;
        //allows the code to continue if there is space avalible in the table to be filled
        sem_wait(&sharedMemory->isNotFull);
    
        //standard mutex lock to keep critical section clear.
        sem_wait(&sharedMemory->mutexLock);

        std::cout << "Produced: " << randomValue << std::endl;

        //Places produced item into table.
        sharedMemory->table[sharedMemory->in] = randomValue;
        //increments in flag.
        sharedMemory->in = (sharedMemory->in + 1) % TABLE_SIZE;

        sem_post(&sharedMemory->mutexLock);

        //Increments isNotEmpty because there is now a full space.
        sem_post(&sharedMemory->isNotEmpty);


        sleep(SLEEP_TIME);
    }
    
    shm_unlink("shmpath");
    return 0;
}