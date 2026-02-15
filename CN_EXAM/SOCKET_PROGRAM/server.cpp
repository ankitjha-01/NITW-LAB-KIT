#include <iostream>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    signal(SIGCHLD, SIG_IGN); // Prevent zombie processes

    // 1. create socket
    int server = socket(AF_INET, SOCK_STREAM, 0); // AF_INET: IPv4, SOCK_STREAM: TCP, 0: default protocol
    struct sockaddr_in addr = {AF_INET, htons(8080), {INADDR_ANY}}; // IPv4, port 8080, bind to all interfaces

    // 2. bind socket to address
    bind(server, (struct sockaddr*)&addr, sizeof(addr));

    // 3. listen for incoming connections
    listen(server, 10); // max 10 pending connections in the queue
    cout << "Server listening on port 8080..." << endl;

    int client, count = 0; // To keep track of client count

    // 4. accept and handle clients
    while (client = accept(server, 0, 0)) {
        count++;
        cout << "Client " << count << " Connected" << endl;

        // 5. fork a child process to handle the client
        if (!fork()) { // Child process
            close(server); // Child doesn't need the listening socket
            char buf[1024]; // Buffer to receive data
            int n1, n2; char op;
            
            // Communication loop with the client
            while (recv(client, buf, 1024, 0) > 0) {
                // Parse the received expression
                sscanf(buf, "%d %c %d", &n1, &op, &n2);

                int res = (op=='+')?n1+n2 : (op=='-')?n1-n2 : (op=='*')?n1*n2 : (n2?n1/n2 : 0);
                string s = to_string(res);
                send(client, s.c_str(), s.length()+1, 0); // Send result back to client
            }

            cout << "Client " << count << " Disconnected" << endl;
            exit(0); // Terminate child process
        }
        
        close(client); // Parent doesn't need the client socket
    }

    close(server); // Close the listening socket
    return 0;
}