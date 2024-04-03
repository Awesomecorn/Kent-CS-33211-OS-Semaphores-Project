#include "./producer.hpp"
#include "./memoryspace.hpp"


int main(int argc, char *argv[]){
    struct memorySpace *producerMem;

    int fd = shm_open(SHMPATH, O_RDWR, 0);
    std::cout << "fd: " << fd << std::endl;
    ftruncate(fd, sizeof(*producerMem));

    producerMem = (struct memorySpace *)mmap(NULL, sizeof(*producerMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_init(&(producerMem->sem), 1,1);
    
    sem_init(&(producerMem->full), 1,0);
    
    sem_init(&(producerMem->empty), 1, TABLE_SIZE);

    for (size_t i = 0; i < 4; i++)
    {
        sem_wait(&producerMem->empty);
        sem_wait(&producerMem->sem);
        int randomValue = rand() % 100;
        std::cout << "Produced: " << randomValue << std::endl;

        producerMem->table[producerMem->in] = randomValue;


        producerMem->in = (producerMem->in + 1) % TABLE_SIZE;

        sem_post(&producerMem->sem);
        sem_post(&producerMem->full);
    }
    

    return 0;
}