#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 5555
#define ADDRESS "127.0.0.1"
#define MAX_CONNECTIONS 10
#define BUF_SIZE 1001

int main(void) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr, remote_addr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    char buffer[BUF_SIZE];
    memset(&addr, 0, socklen);
    memset(&remote_addr, 0, socklen);

    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_aton(ADDRESS, &addr.sin_addr);

    if(bind(sockfd,(const struct sockaddr *)&addr, socklen)==-1) {
        perror("bind");
        return 1;
    }

    if(listen(sockfd,MAX_CONNECTIONS)==-1) {
        perror("listen");
        return 1;
    }
    
    while(1) {
        int csockfd = accept(sockfd,(struct sockaddr*)&remote_addr, &socklen);
        int n;
        while(n=recv(csockfd, buffer, BUF_SIZE-1, 0)) {
            printf("%s > %s", inet_ntoa(remote_addr.sin_addr), buffer);
        }
    }
}
