#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 7070

int main() {
    int sockfd, newfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char buffer[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

    // Read until client shuts down write
    read(newfd, buffer, sizeof(buffer));
    printf("Server received: %s\n", buffer);

    char msg[] = "Server response\n";
    write(newfd, msg, strlen(msg));

    close(newfd);
    close(sockfd);
}
