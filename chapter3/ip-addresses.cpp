//
// Created by Maximilian Kuttner on 08.07.2026.
//
/*
Compile:
    gcc structs_demo.c -o structs_demo

Run:
    ./structs_demo
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <netinet/in.h>

int main(void)
{
    /*
     * 1. IPv4: struct sockaddr_in + struct in_addr
     */
    struct sockaddr_in ipv4_addr;
    // fill ipv4_addr pointer with zeros
    memset(&ipv4_addr, 0, sizeof ipv4_addr);

    ipv4_addr.sin_family = AF_INET;
    ipv4_addr.sin_port = htons(8080);   // host-to-network short

    // convert a ip4 address string to binary representation
    // returns 1 if the address could be converted and is valid
    // o.w. returns 0
    // Note: pton stands for "presentation to network" or "printable to network"
    if (inet_pton(AF_INET, "192.168.1.42", &ipv4_addr.sin_addr) <= 0) {
        perror("inet_pton IPv4");
        return 1;
    }

    // declare a character array to hold the IPv4 address string
    char ipv4_string[INET_ADDRSTRLEN];

    // convert a binary ip4 address to string representation
    // this is basically the inverse of ``inet_pton``
    inet_ntop(AF_INET,
              &ipv4_addr.sin_addr,
              ipv4_string,
              sizeof ipv4_string);

    printf("IPv4 sockaddr_in example:\n");
    printf("  IP:   %s\n", ipv4_string);
    printf("  Port: %d\n\n", ntohs(ipv4_addr.sin_port));


    /*
     * 2. IPv6: struct sockaddr_in6 + struct in6_addr
     */
    struct sockaddr_in6 ipv6_addr;
    memset(&ipv6_addr, 0, sizeof ipv6_addr);

    ipv6_addr.sin6_family = AF_INET6;
    ipv6_addr.sin6_port = htons(8080);

    if (inet_pton(AF_INET6, "2001:db8::1", &ipv6_addr.sin6_addr) <= 0) {
        perror("inet_pton IPv6");
        return 1;
    }

    char ipv6_string[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET6,
              &ipv6_addr.sin6_addr,
              ipv6_string,
              sizeof ipv6_string);

    printf("IPv6 sockaddr_in6 example:\n");
    printf("  IP:   %s\n", ipv6_string);
    printf("  Port: %d\n\n", ntohs(ipv6_addr.sin6_port));


    /*
     * 3. Generic struct sockaddr cast
     *
     * Socket functions like connect(), bind(), and accept()
     * usually expect struct sockaddr*.
     */
    struct sockaddr *generic_addr;

    generic_addr = (struct sockaddr *)&ipv4_addr;

    printf("Generic sockaddr example:\n");
    printf("  sa_family: %d\n", generic_addr->sa_family);
    printf("  This was originally a struct sockaddr_in.\n\n");


    /*
     * 4. struct sockaddr_storage
     *
     * This can hold either IPv4 or IPv6.
     */
    struct sockaddr_storage storage;
    memset(&storage, 0, sizeof storage);

    memcpy(&storage, &ipv4_addr, sizeof ipv4_addr);

    printf("sockaddr_storage example:\n");

    if (storage.ss_family == AF_INET) {
        struct sockaddr_in *addr = (struct sockaddr_in *)&storage;

        char ip[INET_ADDRSTRLEN];

        inet_ntop(AF_INET,
                  &addr->sin_addr,
                  ip,
                  sizeof ip);

        printf("  Stored IPv4 address: %s\n", ip);
        printf("  Stored IPv4 port:    %d\n\n", ntohs(addr->sin_port));
    }


    /*
     * 5. struct addrinfo + getaddrinfo()
     *
     * This is the modern way to prepare address info.
     */
    struct addrinfo hints;
    struct addrinfo *results;
    struct addrinfo *p;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;       // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;   // TCP
    hints.ai_protocol = 0;             // Any protocol

    int status = getaddrinfo("example.com", "80", &hints, &results);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    printf("getaddrinfo example for example.com:80:\n");

    for (p = results; p != NULL; p = p->ai_next) {
        void *addr;
        char ip_string[INET6_ADDRSTRLEN];

        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &ipv4->sin_addr;
            inet_ntop(AF_INET, addr, ip_string, sizeof ip_string);
            printf("  IPv4: %s\n", ip_string);
        } else if (p->ai_family == AF_INET6) {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &ipv6->sin6_addr;
            inet_ntop(AF_INET6, addr, ip_string, sizeof ip_string);
            printf("  IPv6: %s\n", ip_string);
        }
    }

    freeaddrinfo(results);

    return 0;
}