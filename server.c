#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

int main() {
    int server_socket, client_socket, pid;
    struct sockaddr_in server_address, client_address;
    socklen_t client_len = sizeof(client_address);
    char buffer[MAX_LEN];

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to a specific address and port
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Loop to handle incoming connections
    while (1) {
        // Accept incoming connection
        if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_len)) < 0) {
            perror("accept failed");
            continue;
        }

        // Use fork to handle incoming connection
        pid = fork();
        if (pid == 0) {
            // Child process
            close(server_socket);

            // Read incoming data from client
            memset(buffer, 0, MAX_LEN);
            if (read(client_socket, buffer, MAX_LEN) < 0) {
                perror("read failed");
                exit(EXIT_FAILURE);
            }

            // Combine incoming data with string "from server xyz"
            strcat(buffer, " from server xyz");

            // Write combined data back to client
            if (write(client_socket, buffer, strlen(buffer)) < 0) {
                perror("write failed");
                exit(EXIT_FAILURE);
            }

            close(client_socket);
            exit(EXIT_SUCCESS);
        } else if (pid > 0) {
            // Parent process
            close(client_socket);
        } else {
            perror("fork failed");
        }
    }

    return 0;
}
