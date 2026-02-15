#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

const int NUM_PHILOSOPHERS = 5;

// Shared semaphores (chopsticks)
sem_t chopsticks[NUM_PHILOSOPHERS];

void* philosopher(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; ++i) {
        cout << "Philosopher " << id << " is thinking." << endl;
        usleep(rand() % 500000);

        // Deadlock avoidance: last philosopher picks right first
        if (id == NUM_PHILOSOPHERS - 1) {
            sem_wait(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]);
            sem_wait(&chopsticks[id]);
        } else {
            sem_wait(&chopsticks[id]);
            sem_wait(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]);
        }

        cout << "Philosopher " << id << " is eating." << endl;
        usleep(rand() % 500000);

        sem_post(&chopsticks[id]);
        sem_post(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]);

        cout << "Philosopher " << id << " finished eating." << endl;
    }
    return nullptr;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize semaphores for chopsticks (initial value 1)
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
        sem_init(&chopsticks[i], 0, 1); // name, pshared, value

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        ids[i] = i;
        pthread_create(&threads[i], nullptr, philosopher, &ids[i]);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
        pthread_join(threads[i], nullptr);

    // Destroy semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
        sem_destroy(&chopsticks[i]);

    return 0;
}
