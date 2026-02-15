#include <iostream>
#include <pthread.h>
#include <vector>
using namespace std;

#define SIZE 3  // Change this as needed

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

struct ThreadData {
    int row;
};

// Function executed by each thread
void* multiplyRow(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int row = data->row;

    for (int col = 0; col < SIZE; col++) {
        C[row][col] = 0;
        for (int k = 0; k < SIZE; k++) {
            C[row][col] += A[row][k] * B[k][col];
        }
    }

    pthread_exit(NULL);
}

int main() {
    // Initialize matrices A and B
    cout << "Matrix A:\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = i + j;
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nMatrix B:\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            B[i][j] = i * j;
            cout << B[i][j] << " ";
        }
        cout << endl;
    }

    // Create one thread per row
    pthread_t threads[SIZE];
    ThreadData tdata[SIZE];

    for (int i = 0; i < SIZE; i++) {
        tdata[i].row = i;
        pthread_create(&threads[i], NULL, multiplyRow, &tdata[i]);
    }

    // Wait for all threads
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print result
    cout << "\nResult Matrix C (A x B):\n";
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
