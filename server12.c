#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define SERV_PORT 10010
#define LISTENQ 8

void handleRequest(char *receivePacket, char *sendPacket);
void numToByteArray(int32_t num, int size, char*bArray);

int main() {
	
	int listenfd, connfd, n;
	socklen_t clilen;
	char buf[9];
	struct sockaddr_in cliaddr, servaddr;
	char sendPacket[14];

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

		while((n = recv(connfd, buf, 9, 0)) > 0) {
			handleRequest(buf, sendPacket);
			send(connfd, sendPacket, 14, 0);

			bzero(buf, 9);
			bzero(sendPacket, 14);
		}

		if (n < 0) {
			perror("Read error");
			exit(1);
		}

		close(connfd);
	}
	close (listenfd);

	
}


void numToByteArray(int32_t num, int size, char*bArray) {
	int i;
	for (i = 0; i < size; i++) {
		bArray[i] = (num >> (i * 8)) & 0xFF;
	}
}

void handleRequest(char *receivePacket, char *sendPacket) {
	char c = receivePacket[0];
	int32_t a = 0;
	int32_t b = 0;
	int32_t answer = 0;
	char valid = '1';
	int i;
	char answerArray[4];

	// part a:
	for (i = 4; i >= 1; i--) {
		a += receivePacket[i];
		if (i > 1) {
			a = a << 8;
		}
	}

	// part b:
	for (i = 8; i >= 5; i--) {
		b += receivePacket[i];
		if (i > 5) {
			b = b << 8;
		}
	}

	switch (c) {
		case '+':
			answer = a + b;
			break;
		case '-':
			answer = a - b;
			break;
		case 'x':
			answer = a * b;
			break;
		case '/':
			answer = a / b;
			break;
		default:
			break;
	}
	
	memcpy(sendPacket, receivePacket, 9);

	numToByteArray(answer, 4, answerArray);
	memcpy(sendPacket + 9, answerArray, 4);
	
	if (a < b && c == '-') {
		valid = '2';
	}
	sendPacket[13] = valid;

	// handle potential unsigned arithmetic problems. too big, divide by zero, etc
	
}
