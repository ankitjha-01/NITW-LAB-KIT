#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;

#define THREAD_LIMIT 2   // Number of threads per merge call

int a[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};  // Global array
int n = sizeof(a)/sizeof(a[0]);

typedef struct {
    int low;
    int high;
} Range;

void merge(int low, int mid, int high) {
    int n1 = mid - low + 1;
    int n2 = high - mid;
    int left[n1], right[n2];

    for (int i = 0; i < n1; i++)
        left[i] = a[low + i];
    for (int i = 0; i < n2; i++)
        right[i] = a[mid + 1 + i];

    int i = 0, j = 0, k = low;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }
    while (i < n1) a[k++] = left[i++];
    while (j < n2) a[k++] = right[j++];
}

void *mergesort(void *arg) {
    Range *r = (Range *)arg;
    int low = r->low;
    int high = r->high;

    if (low >= high)
        return NULL;

    int mid = (low + high) / 2;

    Range left = {low, mid};
    Range right = {mid + 1, high};

    pthread_t threads[THREAD_LIMIT];

    // Create threads for left and right halves
    pthread_create(&threads[0], NULL, mergesort, &left);
    pthread_create(&threads[1], NULL, mergesort, &right);

    // Wait for both halves to finish
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    // Merge sorted halves
    merge(low, mid, high);
    return NULL;
}

int main() {
    Range total = {0, n - 1};
    pthread_t t;

    pthread_create(&t, NULL, mergesort, &total);
    pthread_join(t, NULL);

    cout << "Sorted array: " << endl;
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << endl;
    return 0;
}
