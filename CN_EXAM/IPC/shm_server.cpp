#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

using namespace std;

int main() {
    int key = ftok("key", 1);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char*)shmat(shmid, nullptr, 0);
    
    while (true) {
        cout << "Waiting for message..." << endl;
        while (str[0] == '\0') sleep(1);

        cout << "Received message: " << str << endl;
        str[0] = '\0';
    }

    shmdt(str);
    shmctl(shmid, IPC_RMID, nullptr);
}