#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXLINE 4096
#define SERV_PORT 10010

int main(int argc, char **argv) {
	int sockfd;
	struct sockaddr_in servaddr;
	struct hostent *he;
	char sendline[MAXLINE], recvline[MAXLINE];
/*	char c;
	unsigned int a;
	unsigned int b;
    
	// a: 0-3, b: 4-7, c: 8
	char msg[9] = {(char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0};
*/
	if (argc != 2) {
        	perror("Usage: TCPClient <IP address of the server");
        	exit(1);
	}
    

  /*  	printf("\nEnter first number(>= 0): ");
    	scanf(" %u", &msg[0]);
    	printf("\nEnter second number(>= 0): ");
    	scanf(" %u", &msg[4]);
    	printf("\nEnter operator: ");
    	scanf(" %c", &msg[8]);

	//printf(" \n%p", *msg);
*/    
	if ((he = gethostbyname(argv[1])) == NULL) {
		perror("gethostbyname");
		exit(1);
	}
    	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        	perror("Problem in creating the socket");
        	exit(2);
    	}

    	memset(&servaddr, 0, sizeof(servaddr));
    	servaddr.sin_family = AF_INET;
    	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    	servaddr.sin_port = htons(SERV_PORT);

    	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        	perror("Problem in connecting to the server");
        	exit(3);
    	}

    	while (fgets(sendline, MAXLINE, stdin) != NULL) {
        	send(sockfd, sendline, strlen(sendline), 0);

        	if (recv(sockfd, recvline, MAXLINE, 0) == 0) {
            		perror("The server terminated prematurely");
            		exit(4);
        	}
        	printf("String received from the server: ");
		recvline[strlen(recvline)] = '\0';
        	fputs(recvline, stdout);
    	}

    	return 0;
}
