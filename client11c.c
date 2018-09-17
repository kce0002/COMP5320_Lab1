#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#define SERV_PORT 10010
#define MAXLINE 1024
#define MAXDIGITS 5
#define NUM_AMOUNT 10000

int main(int argc, char **argv) {
	
	if (argc != 2) {
		perror("Usage: UDPClient <IP address of the server>");
		exit(1);
	}
	
	char numbers[NUM_AMOUNT + 1][MAXDIGITS + 1];
	int i;
	char temp[MAXDIGITS + 1];
	int socketfd;
	int n;
	int servlen;
	struct sockaddr_in servaddr;
	
	for (i = 1; i <= NUM_AMOUNT; i++) {
		sprintf(temp, "%d", i);
		strcpy(numbers[i - 1], temp);
	}
	numbers[NUM_AMOUNT][MAXDIGITS] = '\0';
	
	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Problem in creating the socket");
		exit(2);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(SERV_PORT);
	
	for (i = 0; i < NUM_AMOUNT; i++) {
		printf("Sending %s\n", numbers[i]);
		sendto(socketfd, (char *) numbers[i], strlen(numbers[i]), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr));
	}

	close(socketfd);



	return 0;
}
