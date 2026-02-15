#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main() {
    const char* fifo_path = "/tmp/myfifo";
    char buffer[256];

    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        perror("open fifo");
        return 1;
    }

    while (true) {
        int n = read(fd, buffer, sizeof(buffer));
        if (n <= 0) continue;
        buffer[n] = '\0';
        if (strcmp(buffer, "exit") == 0) break;
        std::cout << "Reader received: " << buffer << std::endl;
    }

    close(fd);
    unlink(fifo_path);
    std::cout << "Reader exiting..." << std::endl;
    return 0;
}
