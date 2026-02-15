#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // --- Child Process ---
        std::cout << "Child (PID: " << getpid() << ") is running a new program." << std::endl;
        execlp("ls", "ls", "-l", nullptr); // name of file, arg0, arg1, ..., nullptr
        
        // execvp only returns if an error occurred
        perror("execvp failed");
        exit(1);
    } else {
        // --- Parent Process ---
        std::cout << "Parent (PID: " << getpid() << ") is waiting for child " << pid << "." << std::endl;
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to terminate
        
        if (WIFEXITED(status)) {
            std::cout << "Parent: Child finished with exit code " << WEXITSTATUS(status) << "." << std::endl;
        }
    }
    return 0;
}