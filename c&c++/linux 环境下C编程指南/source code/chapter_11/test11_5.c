#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/poll.h>
#include <stdlib.h>
#include <string.h>

#define SERV_PORT 36789

#ifndef OPEN_MAX
#define OPEN_MAX 1024
#endif

#ifndef MAXLINE
#define MAXLINE 1024
#endif
#ifndef INFTIM
#define INFTIM -1
#endif

int main(int argc, char **argv)
{
    int i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, 16);

    client[0].fd = listenfd;
    client[0].events = POLLIN;
    for (i = 1; i < OPEN_MAX; i++)
        client[i].fd = -1; /* -1 indicates available entry */
    maxi = 0; /* max index into client[] array */
    for ( ; ; ) {
        nready = poll(client, maxi + 1, INFTIM);

        if (client[0].revents & POLLIN) { /* new client connection */
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

            for (i = 1; i < OPEN_MAX; i++)
                if (client[i].fd < 0) {
                    client[i].fd = connfd; /* save descriptor */
                    break;
                }
            if (i == OPEN_MAX)
                printf("too many clients\n");
            client[i].events = POLLIN;
            if (i > maxi)
                maxi = i; /* max index in client[] array */

            if (--nready <= 0)
                continue; /* no more readable descriptors */
        }

        for (i = 1; i <= maxi; i++) { /* check all clients for data */
            if ( (sockfd = client[i].fd) < 0)
                continue;
            if (client[i].revents & (POLLIN | POLLERR)) {
                if ( (n = read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        /* connection reset by client */
                        close(sockfd);
                        client[i].fd = -1;
                    } else
                        printf("read error\n");
                } else if (n == 0) {
                    /* connection closed by client */
                    close(sockfd);
                    client[i].fd = -1;
                } else
                    write(sockfd, buf, n);
                if (--nready <= 0)
                    break; /* no more readable descriptors */
            }
        }
    }
}
