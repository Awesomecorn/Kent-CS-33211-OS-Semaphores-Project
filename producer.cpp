#include "./producer.hpp"
#include "./memoryspace.hpp"


int main(int argc, char *argv[]){
    int fd;
    struct memorySpace *producerMem;

    fd = shm_open("shmpath", O_CREAT | O_EXCL |O_RDWR, 0600);


    ftruncate(fd, sizeof(*producerMem));

    producerMem = static_cast<memorySpace*>(mmap(NULL, sizeof(*producerMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    sem_init(&(producerMem->sem), 1,1);
    
    sem_init(&(producerMem->full), 1,0);
    
    sem_init(&(producerMem->empty), 1, TABLE_SIZE);

    producerMem->in = 0;
    producerMem->out = 0;

    for (size_t i = 0; i < 10; i++)
    {
        sem_wait(&producerMem->empty);
        sem_wait(&producerMem->sem);
        int randomValue = i;
        std::cout << "Produced: " << randomValue << std::endl;

        producerMem->table[producerMem->in] = randomValue;


        producerMem->in = (producerMem->in + 1) % TABLE_SIZE;

        sem_post(&producerMem->sem);
        sem_post(&producerMem->full);
        sleep(5);
    }
    
    shm_unlink("shmpath");

    return 0;
}