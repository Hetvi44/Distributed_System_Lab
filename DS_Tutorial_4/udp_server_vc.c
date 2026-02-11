#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define VC_SIZE 4   // P0=Server, P1,P2,P3=Clients

typedef struct {
    int pid;
    int vc[VC_SIZE];
} Message;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int main() {

    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    Message msg;
    int server_vc[VC_SIZE] = {0};  // Server vector clock (P0)

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    printf("UDP Server running with infinite loop...\n");

    // ✅ Infinite loop (as required)
    while (1) {

        recvfrom(sockfd, &msg, sizeof(msg), 0,
                 (struct sockaddr*)&cliaddr, &len);

        // Update server vector clock using max rule
        for (int i = 0; i < VC_SIZE; i++) {
            server_vc[i] = max(server_vc[i], msg.vc[i]);
        }

        // Increment server’s own clock (P0 index = 0)
        server_vc[0]++;

        printf("\nReceived from Client %d\n", msg.pid);
        printf("Server Vector Clock: ");
        for (int i = 0; i < VC_SIZE; i++) {
            printf("%d ", server_vc[i]);
        }
        printf("\n");

        // Copy updated server vector clock into message
        for (int i = 0; i < VC_SIZE; i++) {
            msg.vc[i] = server_vc[i];
        }

        // Echo back to client
        sendto(sockfd, &msg, sizeof(msg), 0,
               (struct sockaddr*)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}
