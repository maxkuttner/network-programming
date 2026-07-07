#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

int main() {

    // define an unsigned 16-bit integer
    uint16_t port = 8080;

    // conver to network byte order (i.e. big endian / most significant byte first)
    uint16_t network = htons(port);

    unsigned char *hostBytes = (unsigned char *)&port;
    unsigned char *netBytes  = (unsigned char *)&network;

    printf("Host bytes:\n");
    printf("%02X %02X\n",
           hostBytes[0],
           hostBytes[1]);

    printf("\nNetwork bytes:\n");
    printf("%02X %02X\n",
           netBytes[0],
           netBytes[1]);
}


