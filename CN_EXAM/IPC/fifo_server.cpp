#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main() {
    const char* fifo_path = "/tmp/myfifo"; // Path to the FIFO file
    mkfifo(fifo_path, 0666); // Create the FIFO file with read/write permissions

    char buffer[1024];
    while (true) {
        int fd = open(fifo_path, O_RDONLY);

        int bytes_read = read(fd, buffer, sizeof(buffer)); // read from the fifo
        if (bytes_read > 0) {
            cout << "Received message: " << buffer << endl;
        }
        close(fd); // Close the FIFO file descriptor
    }
    return 0;
}