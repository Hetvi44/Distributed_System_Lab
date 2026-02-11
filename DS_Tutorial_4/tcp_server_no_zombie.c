#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT 9090

// SIGCHLD handler
void reap_child(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    int sockfd, newfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    // Handle zombie processes
    signal(SIGCHLD, reap_child);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);

    printf("TCP Server running (no zombies)...\n");

    while (1) {
        newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

        if (fork() == 0) {
            // Child
            close(sockfd);
            char msg[] = "Hello Client\n";
            write(newfd, msg, strlen(msg));
            close(newfd);
            exit(0);
        }
        // Parent
        close(newfd);
    }
}
