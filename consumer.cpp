#include "./consumer.hpp"
#include "./memoryspace.hpp"


int main(int argc, char *argv[]){
    struct memorySpace *consMem;

    int fd = shm_open("shmpath", O_RDWR, 0);

    ftruncate(fd, sizeof(*consMem));

    consMem = (struct memorySpace *)mmap(NULL, sizeof(*consMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for (size_t i = 0; i < 10; i++)
    {
        sem_wait(&consMem->full);
        sem_wait(&consMem->sem);

        std::cout << "Consumed: " << consMem->table[consMem->out] << std::endl;
        consMem->out = (consMem->out + 1) % TABLE_SIZE;

        sem_post(&consMem->sem);
        sem_post(&consMem->empty);

        sleep(10);
    

    }

    shm_unlink("shmpath");

    return 0;
}