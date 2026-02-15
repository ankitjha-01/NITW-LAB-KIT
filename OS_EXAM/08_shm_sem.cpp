#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_sem"
#define SHM_SIZE 1024

int main() {
    // Create shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    char* data = (char*)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Create a named semaphore, initial value 0
    sem_t* sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);

    if (fork() == 0) { // Child writes
        std::cout << "CHILD: Writing to shared memory..." << std::endl;
        strcpy(data, "Hello from shared memory!");
        std::cout << "CHILD: Signaling parent." << std::endl;
        sem_post(sem); // Signal parent
        munmap(data, SHM_SIZE);
        close(shm_fd);
    } else { // Parent reads
        std::cout << "PARENT: Waiting for signal from child..." << std::endl;
        sem_wait(sem); // Wait for child
        std::cout << "PARENT: Read from shared memory: '" << data << "'" << std::endl;

        wait(NULL); // Wait for child to finish

        // Cleanup
        munmap(data, SHM_SIZE);
        close(shm_fd);
        shm_unlink(SHM_NAME);

        sem_close(sem);
        sem_unlink(SEM_NAME);
    }
    return 0;
}
