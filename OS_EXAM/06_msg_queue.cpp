#include <iostream>
#include <mqueue.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    const char* mq_name = "/my_queue";
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;

    // Unlink any previous queue with the same name
    mq_unlink(mq_name);
    mqd_t mq = mq_open(mq_name, O_CREAT | O_RDWR, 0666, &attr);

    if (fork() == 0) { // Sender process
        const char* messages[] = {"Hello", "Message Queue", "Testing", "exit"};
        for (int i = 0; i < 4; i++) {
            mq_send(mq, messages[i], strlen(messages[i]) + 1, 0);
            std::cout << "SENDER: Sent '" << messages[i] << "'" << std::endl;
            sleep(1); // simulate delay
        }
        mq_close(mq);
    } else { // Receiver process
        char buffer[256];
        while (true) {
            ssize_t bytes = mq_receive(mq, buffer, 256, NULL);
            if (bytes >= 0) {
                buffer[bytes] = '\0';
                std::cout << "RECEIVER: Got '" << buffer << "'" << std::endl;
                if (strcmp(buffer, "exit") == 0) break; // exit condition
            }
        }
        wait(NULL);
        mq_close(mq);
        mq_unlink(mq_name);
    }

    return 0;
}
