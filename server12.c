#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096
#define SERV_PORT 10010
#define LISTENQ 8
int main() {
	
	int listenfd, connfd, n;
	socklen_t clilen;
	char buf[MAXLINE];
	struct sockaddr_in cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);
	printf("Server running... waiting for connections.\n");

	while(1) {

		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
		printf("\nReceived request...\n");

		while((n = recv(connfd, buf, MAXLINE, 0)) > 0) {
			printf("String received from the client: ");
			buf[n] = '\0';
			puts(buf);
			send(connfd, "Hello from the server.", 100, 0);
		}

		if (n < 0) {
			perror("Read error");
			exit(1);
		}

		close(connfd);
	}
	close (listenfd);

	
}
