#include <iostream>
#include <pthread.h>
#include <unistd.h>

// Shared resources
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool message_ready = false;

void* writer(void*) {
    sleep(1); // Simulate some work
    pthread_mutex_lock(&mtx);
    std::cout << "Writer: Locking mutex and preparing message." << std::endl;
    message_ready = true;
    std::cout << "Writer: Signaling the reader." << std::endl;
    pthread_cond_signal(&cond); // Signal the waiting thread
    pthread_mutex_unlock(&mtx);
    return NULL;
}

void* reader(void*) {
    pthread_mutex_lock(&mtx);
    std::cout << "Reader: Locking mutex." << std::endl;
    while (!message_ready) {
        std::cout << "Reader: Message not ready, waiting on condition..." << std::endl;
        // Atomically unlocks mutex and waits. Re-locks mutex upon wakeup.
        pthread_cond_wait(&cond, &mtx);
    }
    std::cout << "Reader: Condition met! Processing message." << std::endl;
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main() {
    pthread_t read_thread, write_thread;
    pthread_create(&read_thread, NULL, reader, NULL);
    pthread_create(&write_thread, NULL, writer, NULL);
    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);
    // pthread_detach(any_thread);
    return 0;
}