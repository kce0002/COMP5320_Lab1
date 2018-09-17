// Kyle Ehlers
// Mitchell Derisio
// COMP 5320
// Lab 1.1
// client11b.c
/* 
 * Client side implementation of UDP Echo server.
 *
 * This program takes a server IP Address as a command line argument,
 * prompts the user for a message, sends the message to the server, 
 * and waits for the server to respond with the same message. It 
 * also keeps track of the round trip time of the message being sent
 * and received.
*/
// TODO: Find out if program is supposed to use IP Address or hostname.
// TODO: Figure out why first attempt fails.


// Library includes:
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

// Constants:
#define SERV_PORT 10010
#define MAXLINE 1024

int main(int argc, char **argv) {
	
	// Check for proper usage:
	if (argc != 2) {
		perror("Usage: UDPClient <IP address of the server>");
		exit(1);
	}
	
	// Variable declarations:
	int socketfd;
	int n;
	int servlen;
	struct sockaddr_in servaddr;
	char sendline[MAXLINE], recvline[MAXLINE];
	struct timeval start, end;
	int rtt;
	
	// Prompt user for message:
	printf("\nEnter message to be sent to server: ");
	fgets(sendline, 1024, stdin);

	// Create the UDP socket:
	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Problem in creating the socket");
		exit(2);
	}
	
	// Prepare additional socket information:
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(SERV_PORT);
	
	gettimeofday(&start, NULL); // Start "timer"
	sendto(socketfd, (char *) sendline, strlen(sendline), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr));

	n = recvfrom(socketfd, (char *) recvline, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &servlen);
		
	gettimeofday(&end, NULL); // End "timer"

	rtt = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000); // Calculate RTT	
	
	recvline[n] = '\0';
	
	// Output(stdout) echoed message and RTT:
	printf("%s\n", recvline);
	printf("Round Trip Time (RTT) = %d(ms)\n", rtt);
	
	// Close socket:
	close(socketfd);

	return 0;
}
