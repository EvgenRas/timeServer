#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

const int MAXLINE = 1024;

int main (int argc, char **argv)
{
    int listenfd, connfd;
    char buff[MAXLINE];
    struct sockaddr_in servaddr;
    time_t ticks;

    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror ("socket");
        exit(1);
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind (listenfd,(sockaddr*)&servaddr, sizeof(servaddr) ) < 0)
    {
        perror("server error");
        exit(1);
    }

    if ( listen(listenfd, SOMAXCONN) < 0 )
    {
        perror ("server");
        exit(1);
    }

    for(;;)
    {
        connfd = accept(listenfd, (sockaddr *)NULL, NULL);

        ticks = time(NULL);
        snprintf ( buff, sizeof(buff), "%.24s\r\n", ctime(&ticks) );
        write ( connfd, buff, strlen(buff) );
        close(connfd);
    }

    exit(0);
}
