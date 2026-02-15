#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

// Signal handler function
void handler(int signum) {
    cout << "Child: Received signal SIGUSR1 (" << signum << ")!" << endl;
}

int main() {
    pid_t pid = fork();

    if (pid == 0) { // Child process
        signal(SIGUSR1, handler);  // Register signal handler
        // struct sigaction sa;
        // sa.sa_handler = handler;      // Set handler function
        // sigemptyset(&sa.sa_mask);     // Don’t block any signals during handler
        // sa.sa_flags = 0;              // No special flags

        // if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        //     perror("sigaction");
        //     return 1;
        // }

        cout << "Child: Waiting for SIGUSR1 from parent..." << endl;
        while (true) {
            pause(); // Wait for signal
        }
    } else { // Parent process
        sleep(2); // Give child time to set up handler
        cout << "Parent: Sending SIGUSR1 to child." << endl;
        kill(pid, SIGUSR1);

        sleep(2);
        cout << "Parent: Sending SIGTERM to end child." << endl;
        kill(pid, SIGTERM);
        wait(NULL);
    }

    return 0;
}
