#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define VC_SIZE 4

typedef struct {
    int pid;
    int vc[VC_SIZE];
} Message;

int max(int a, int b) {
    return a > b ? a : b;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <client_id>\n", argv[0]);
        exit(1);
    }

    int pid = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in servaddr;
    Message msg;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(msg.vc, 0, sizeof(msg.vc));
    msg.pid = pid;

    for (int i = 0; i < 5; i++) {
        msg.vc[pid]++;  // Client event

        sendto(sockfd, &msg, sizeof(msg), 0,
               (struct sockaddr*)&servaddr, sizeof(servaddr));

        recvfrom(sockfd, &msg, sizeof(msg), 0, NULL, NULL);

        msg.vc[pid]++;  // Receive event

        printf("Client %d received echo\nVector Clock: ", pid);
        for (int j = 0; j < VC_SIZE; j++)
            printf("%d ", msg.vc[j]);
        printf("\n");

        sleep(5);
    }

    close(sockfd);
}
