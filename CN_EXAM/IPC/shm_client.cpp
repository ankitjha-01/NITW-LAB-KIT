#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>

using namespace std;

int main() {
    int key = ftok("key", 1);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *str = (char*)shmat(shmid, nullptr, 0);

    while (true) {
        string temp_input;
        cout << "Enter message to send: ";
        getline(cin, temp_input);
        strcpy(str, temp_input.c_str());

        cout << "Message sent: " << str << endl;
    }

    shmdt(str);
}