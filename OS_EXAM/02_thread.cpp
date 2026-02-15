#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

void* task(void* arg) {
    int id = *(int*)arg;
    cout << "Thread " << id << " started\n";
    sleep(1);
    cout << "Thread " << id << " finished\n";

    // Return the thread id
    int* ret = new int(id);
    return (void*)ret;
}

int main() {
    const int N = 3;
    pthread_t threads[N];
    int ids[N];

    // Create threads
    for(int i = 0; i < N; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, task, &ids[i]); 
    }

    // Wait for all threads and get returned id
    for(int i = 0; i < N; i++) {
        void* ret_val;
        pthread_join(threads[i], &ret_val); // or NULL if not needed
        int* ret_id = (int*)ret_val;
        cout << "Main thread: Thread returned id = " << *ret_id << endl;
        delete ret_id; // Free allocated memory
    }

    cout << "Main thread done.\n";
    return 0;
}
