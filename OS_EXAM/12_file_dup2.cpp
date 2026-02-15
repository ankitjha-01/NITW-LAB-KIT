#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
using namespace std;

int main() {
    int fd = open("sample.txt", O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fd == -1) {
        perror("Error in opening file");
        return 1;
    }

    int n;
    cout << "Enter a number: ";
    cin >> n;

    int prevOutput = dup(1); // save current stdout

    dup2(fd, 1); // map stdout(1) to file descriptor

    cout << "This will go to the file 'sample.txt'! " << n << endl;

    // retstore original stdout
    dup2(prevOutput, 1);
    close(prevOutput);

    cout << "This will go to the terminal!" << endl;

    close(fd);
    return 0;
}
