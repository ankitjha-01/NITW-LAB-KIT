#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS   20

// Shared buffer structure
typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
} shared_data_t;

// Shared memory pointer
shared_data_t *shared;

// Named semaphores (outside shared memory)
sem_t *mutex;
sem_t *empty_slots;
sem_t *full_slots;

void producer() {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        sem_wait(empty_slots); // Wait for empty slot
        sem_wait(mutex);       // Lock buffer

        shared->buffer[shared->in] = i;
        printf("Produced: %d\n", i);
        shared->in = (shared->in + 1) % BUFFER_SIZE;

        sem_post(mutex);       // Unlock buffer
        sem_post(full_slots);  // Signal full slot

        usleep(rand() % 100000);
    }
}

void consumer() {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        sem_wait(full_slots);  // Wait for full slot
        sem_wait(mutex);       // Lock buffer

        int item = shared->buffer[shared->out];
        printf("Consumed: %d\n", item);
        shared->out = (shared->out + 1) % BUFFER_SIZE;

        sem_post(mutex);       // Unlock buffer
        sem_post(empty_slots); // Signal empty slot

        usleep(rand() % 150000);
    }
}

int main() {
    srand(time(NULL));

    // 1. Create shared memory using mmap
    shared = (shared_data_t*) mmap(NULL, sizeof(shared_data_t),
                  PROT_READ | PROT_WRITE,
                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    shared->in = 0;
    shared->out = 0;

    // 2. Create named semaphores (outside shared memory)
    mutex       = sem_open("/mutex_sem", O_CREAT | O_EXCL, 0666, 1);
    empty_slots = sem_open("/empty_sem", O_CREAT | O_EXCL, 0666, BUFFER_SIZE);
    full_slots  = sem_open("/full_sem",  O_CREAT | O_EXCL, 0666, 0);

    if (mutex == SEM_FAILED || empty_slots == SEM_FAILED || full_slots == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    // 3. Fork consumer process
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child: Consumer
        consumer();

        // Close semaphores in child
        sem_close(mutex);
        sem_close(empty_slots);
        sem_close(full_slots);
        exit(0);
    } else {
        // Parent: Producer
        producer();

        // Wait for child
        wait(NULL);

        // Cleanup: Close and unlink semaphores
        sem_close(mutex);
        sem_close(empty_slots);
        sem_close(full_slots);

        sem_unlink("/mutex_sem");
        sem_unlink("/empty_sem");
        sem_unlink("/full_sem");

        // Unmap shared memory
        munmap(shared, sizeof(shared_data_t));
    }

    return 0;
}
