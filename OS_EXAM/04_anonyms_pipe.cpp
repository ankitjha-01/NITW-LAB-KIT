#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    int pipefd[2]; // pipefd[0] read, pipefd[1] write
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) { // Child reads
        close(pipefd[1]); // Close write end
        char buffer[100];
        while (true) {
            ssize_t n = read(pipefd[0], buffer, sizeof(buffer));
            if (n <= 0) continue;
            buffer[n] = '\0';
            if (strcmp(buffer, "exit") == 0) break;
            std::cout << "CHILD received: " << buffer << std::endl;
        }
        close(pipefd[0]);
        std::cout << "Child exiting..." << std::endl;
    } else { // Parent writes
        close(pipefd[0]); // Close read end
        const char* messages[] = {
            "Hello",
            "How are you?",
            "This is automatic message",
            "exit" // last message to stop
        };
        for (const char* msg : messages) {
            write(pipefd[1], msg, strlen(msg) + 1);
            sleep(1); // simulate delay
        }
        close(pipefd[1]);
        wait(NULL);
        std::cout << "Parent exiting..." << std::endl;
    }
    return 0;
}
