//
// Created by Maximilian Kuttner on 11.07.2026.
//

#include <iostream>
#include <netdb.h>
#include <ostream>
#include <arpa/inet.h>

int main () {

    // what are hints? -> hints are
    struct addrinfo hints;
    struct addrinfo* results;

    memset(&hints, 0, sizeof(hints));

    // set the address info family to an unspecified
    // family - other options are IPV4, IPV6, ...
    // when it is unspecified it can mean either V4 or V6
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;  // we set the type of the socket to a tcp socket
    hints.ai_flags = AI_PASSIVE; // this will make the socket suitable for calling bind()
    hints.ai_protocol = 0; // any protocol

    const char* test_address_p = "localhost";
    const char* test_port_p = "80";

    int status = getaddrinfo(test_address_p, test_port_p, &hints, &results);
    if (status != 0) {
        std::cout << "getaddrinfo error: " << gai_strerror(status) << std::endl;
    }

    std::cout << "getaddrinfo example for " << test_address_p << ":" << std::endl;
    for (addrinfo *p = results; p != NULL; p = p->ai_next) {
        void *addr;
        // create a ip string
        char ip_string[INET6_ADDRSTRLEN];
        if (p->ai_family == AF_INET) {
            // sockaddr_in specifies an address-info (ai) socket address
            // and we need to cast the socket address to a socket address, internet style
            struct sockaddr_in* ipv4 = (struct sockaddr_in *) p->ai_addr;

            // sin_addr stands for stuct Internet address
            addr = &ipv4->sin_addr;

            // convert the "intern address struct" to a "representation form"
            // and store it in `ip_string`.
            inet_ntop(AF_INET, addr, ip_string, sizeof ip_string);
            // now - we can print it as a standard string
            std::cout << "  IPv4: " << ip_string << std::endl;
        }
        else if (p->ai_family == AF_INET6) {
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*) p->ai_addr;
            addr = &ipv6->sin6_addr;
            inet_ntop(AF_INET6, addr, ip_string, sizeof ip_string);
            std::cout << "  IPv6: " << ip_string << std::endl;
        }
    }

    return 0;
}


