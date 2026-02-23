#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct msg_buf{
    long type;
    char buf[1024];
} msg;

int main() {
    int key = ftok("key", 1);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    while(true) {
        cout << "Enter message to send: ";
        msg.type = 1;
        cin >> msg.buf;
        msgsnd(msgid, &msg, sizeof(msg), 0);

        cout << "Message sent: " << msg.buf << endl;
    }
}