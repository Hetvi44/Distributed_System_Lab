#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 7070

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    // Send request
    char msg[] = "Hello Server";
    write(sockfd, msg, strlen(msg));

    // Half close: close write side only
    shutdown(sockfd, SHUT_WR);

    // First recv – gets server data
    int n = recv(sockfd, buffer, sizeof(buffer), 0);
    buffer[n] = '\0';
    printf("First recv: %s\n", buffer);

    // Second recv – should return 0 (FIN)
    n = recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Second recv returned: %d\n", n);

    close(sockfd);
}
