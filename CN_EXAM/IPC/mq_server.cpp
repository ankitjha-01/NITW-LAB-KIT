#include<iostream>
#include<sys/ipc.h>
#include<sys/msg.h>

using namespace std;

struct msg_buffer{
    long msg_type;
    char msg_buf[1024];
} msg;

int main() {
    int key = ftok("key", 1);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    while(true) {
        cout << "Waiting for message..." << endl;
        msgrcv(msgid, &msg, sizeof(msg), 1, 0);
        cout << msg.msg_buf << endl;
    }
    
    msgctl(msgid, IPC_RMID, nullptr);
}