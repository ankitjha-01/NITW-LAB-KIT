#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

int main() {
    const char* fifo_path = "/tmp/myfifo";
    mkfifo(fifo_path, 0666); // create FIFO

    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        perror("open fifo");
        return 1;
    }

    const char* messages[] = {
        "Hello",
        "Automatic message",
        "exit"
    };

    for (const char* msg : messages) {
        write(fd, msg, strlen(msg) + 1);
        sleep(1); // simulate delay
    }

    close(fd);
    return 0;
}
