#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>

#define PORT 8080
#define BACKLOG 5
#define MAX_EVENTS 10

int main() {
    // Create a server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, BACKLOG) == -1) {
        perror("listen");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Create a kqueue
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Add the server socket to the kqueue
    struct kevent changes;
    EV_SET(&changes, serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &changes, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        close(serverSocket);
        close(kq);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    struct kevent events[MAX_EVENTS];

    while (1) {
        // Wait for events
        int numEvents = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (numEvents == -1) {
            perror("kevent");
            break;
        }

        // Handle events
        for (int i = 0; i < numEvents; ++i) {
            int fd = events[i].ident;

            if (fd == serverSocket) {
                // New connection on the server socket
                struct sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

                if (clientSocket == -1) {
                    perror("accept");
                    continue;
                }

                printf("Accepted connection from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

                // Add the new client socket to the kqueue
                EV_SET(&changes, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, &changes, 1, NULL, 0, NULL) == -1) {
                    perror("kevent");
                    close(clientSocket);
                    continue;
                }
            } else {
                // Data available to read on a client socket
                char buffer[256];
                ssize_t bytesRead = read(fd, buffer, sizeof(buffer));

                if (bytesRead == -1) {
                    perror("read");
                } else if (bytesRead == 0) {
                    // Connection closed by the client
                    printf("Connection closed on socket %d\n", fd);
                    EV_SET(&changes, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &changes, 1, NULL, 0, NULL);
                    close(fd);
                } else {
                    // Print received data
                    buffer[bytesRead] = '\0';
                    printf("Received data on socket %d: %s", fd, buffer);
                }
            }
        }
    }

    // Clean up
    close(serverSocket);
    close(kq);

    return 0;
}