//
// Created by Maximilian Kuttner on 07.07.2026.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
 What is a file descriptor?
 A file descriptor is merely an integer associated with an **open** file.

*/

int main() {
    // socket(domain = ipv4, type = stream, protocol = default protocol for stream)

    // 1) create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // 2) print the file descriptor of the socket
    printf("Socket FD: %d\n", sockfd);

    return 0;
}