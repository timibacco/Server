/*
 *  WEB SERVER
 * RELEASED UNDER NO GODDAMN LICENCE
 * (C) THE FALCON ENGINEERING TEAM
*/
// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_request(int new_socket) {
    char buffer[BUFFER_SIZE] = {0};
    long valread;
    valread = read(new_socket, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);

    char method[8], path[1024], protocol[16];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    char response[BUFFER_SIZE];

    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0) {
            sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
        } else {
            sprintf(response, "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 9\n\nNot Found");
        }
    } else {
        sprintf(response, "HTTP/1.1 405 Method Not Allowed\nContent-Type: text/plain\nContent-Length: 15\n\nMethod Not Allowed");
    }

    write(new_socket, response, strlen(response));
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d\n", PORT);

    while(1) {
        printf("\nWaiting for a connection...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        handle_request(new_socket);
        close(new_socket);
    }

    return 0;
}