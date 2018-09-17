// Kyle Ehlers
// Mitchell Derisio
// COMP 5320
// Lab 1.1
// server11.c
/*
 * Server side implementation of UDP Echo server.
 * This program waits for UDP messages on port 10010. When it 
 * receives a message, it sends it back to the client.
 *
 */
// TODO: Figure out why first attempt fails.


// Library includes:
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

// Constants:
#define SERVE_PORT 10010
#define MAXLINE 1024

int main() {
	// Variable declarations:
	struct sockaddr_in cliaddr, servaddr;
	int socketfd, n;
	socklen_t clilen;
	char buf[MAXLINE];
	
	// Create the UDP socket:
	socketfd = socket(AF_INET, SOCK_DGRAM, 0);	
	
	// Prepare additional socket information:
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVE_PORT);
	
	// Bind socket to the server's IP Address:
	bind(socketfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	// Run until interrupted:
	while (1) {
		// Wait for UDP message:
		n = recvfrom(socketfd, (char *)buf, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &clilen);
		buf[n] = '\0';
		printf("Client: %s\n", buf);
		
		// Send message back to client:
		sendto(socketfd, (char *)buf, MAXLINE, MSG_CONFIRM, (struct sockaddr *) &cliaddr, clilen);
	}

	return 0;
}
