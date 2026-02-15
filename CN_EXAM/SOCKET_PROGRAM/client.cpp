#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

using namespace std;

int main() {
    // 1. Create Socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(8080), inet_addr("127.0.0.1")};

    // 2. Connect to Server
    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    cout << "Connected" << endl;

    // 3. Communication loop
    char buffer[1024];
    while (true) {
        cout << "> ";
        cin.getline(buffer, 1024);
        send(sock, buffer, strlen(buffer), 0);
        if (recv(sock, buffer, 1024, 0) <= 0) break;
        cout << "Result: " << buffer << endl;
    
    }

    close(sock);
    return 0;
}