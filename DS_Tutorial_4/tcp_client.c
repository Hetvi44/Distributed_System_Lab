#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9090

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    read(sockfd, buffer, sizeof(buffer));
    printf("Client received: %s", buffer);

    close(sockfd);
}
