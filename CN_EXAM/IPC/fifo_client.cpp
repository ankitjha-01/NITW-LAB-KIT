#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    const char* fifo_path = "/tmp/myfifo"; // Path to the FIFO file

    while (true) {
        string s;
        cout << "Enter message to send: ";
        cin >> s;

        int fd = open(fifo_path, O_WRONLY); // Open the FIFO for writing
        write(fd, s.c_str(), s.length()+1); // Write the message to the FIFO
        close(fd); // Close the FIFO file descriptor
    }
    return 0;
}