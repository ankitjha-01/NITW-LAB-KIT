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
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3

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

// Producer function
void producer(int id) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        sem_wait(empty_slots);
        sem_wait(mutex);

        int item = id * 100 + i; // Unique item per producer
        shared->buffer[shared->in] = item;
        printf("Producer %d: Produced %d at %d\n", id, item, shared->in);
        shared->in = (shared->in + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(full_slots);

        usleep(rand() % 100000);
    }
}

// Consumer function
void consumer(int id) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        sem_wait(full_slots);
        sem_wait(mutex);

        int item = shared->buffer[shared->out];
        printf("Consumer %d: Consumed %d at %d\n", id, item, shared->out);
        shared->out = (shared->out + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(empty_slots);

        usleep(rand() % 150000);
    }
}

int main() {
    srand(time(NULL));

    // 1. Create shared memory
    shared = (shared_data_t*) mmap(NULL, sizeof(shared_data_t),
                  PROT_READ | PROT_WRITE,
                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
    shared->in = 0;
    shared->out = 0;

    // 2. Create named semaphores
    mutex       = sem_open("/mutex_sem", O_CREAT | O_EXCL, 0666, 1);
    empty_slots = sem_open("/empty_sem", O_CREAT | O_EXCL, 0666, BUFFER_SIZE);
    full_slots  = sem_open("/full_sem",  O_CREAT | O_EXCL, 0666, 0);

    if (mutex == SEM_FAILED || empty_slots == SEM_FAILED || full_slots == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    pid_t pid;
    // 3. Fork producers
    for (int i = 1; i <= NUM_PRODUCERS; i++) {
        pid = fork();
        if (pid == 0) {
            producer(i);
            sem_close(mutex);
            sem_close(empty_slots);
            sem_close(full_slots);
            munmap(shared, sizeof(shared_data_t));
            exit(0);
        }
    }

    // 4. Fork consumers
    for (int i = 1; i <= NUM_CONSUMERS; i++) {
        pid = fork();
        if (pid == 0) {
            consumer(i);
            sem_close(mutex);
            sem_close(empty_slots);
            sem_close(full_slots);
            munmap(shared, sizeof(shared_data_t));
            exit(0);
        }
    }

    // 5. Wait for all children
    for (int i = 0; i < NUM_PRODUCERS + NUM_CONSUMERS; i++) {
        wait(NULL);
    }

    // 6. Cleanup semaphores
    sem_close(mutex);
    sem_close(empty_slots);
    sem_close(full_slots);

    sem_unlink("/mutex_sem");
    sem_unlink("/empty_sem");
    sem_unlink("/full_sem");

    // 7. Cleanup shared memory
    munmap(shared, sizeof(shared_data_t));

    printf("All processes finished. Shared memory and semaphores cleaned up.\n");
    return 0;
}
