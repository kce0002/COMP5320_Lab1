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
#include <netdb.h>
#include <stdint.h>
#include <math.h>

// Constants:
#define SERV_PORT 10010
#define MAXLINE 1024
#define MAXPKTSZ 1038
#define MAX 2

int main(int argc, char **argv) {
	
	// Check for proper usage:
	if (argc != 2) {
		perror("Usage: ./client11b <server hostname>");
		exit(1);
	}
	// Variable declarations:
	int socketfd;
	int n;
	socklen_t servlen;
	struct sockaddr_in servaddr;
	char sendline[MAXLINE], recvline[MAXLINE];
	struct timeval start, end;
	struct hostent *he;
	int rtt;
	//char msg[MAXLINE + 1];
	char msg[MAX + 2];

	int16_t msgLength;
	int32_t seqNum = 0;
	long unsigned int timestamp;
	const unsigned int MAXSEQNUM = (long unsigned int) pow(2, 32) - 1;
	seqNum = (long unsigned int) pow(2, 32) - 3;
	if ((he = gethostbyname(argv[1])) == NULL) {
		perror("Error in resolving server IP Address.");
		exit(2);
	}	
	do {
		if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("Problem in creating the socket.");
			exit(3);
		}
		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr = *((struct in_addr *)he->h_addr);
		servaddr.sin_port = htons(SERV_PORT);
		printf("\nEnter message to be sent to server: ");
		fgets(sendline, MAXLINE, stdin);
		sendline[strlen(sendline) - 1] = '\0';
		if (strlen(sendline) > MAXLINE) {
			printf("Message too long. Try again.\n");
		}
		else {
			gettimeofday(&start, NULL); // Start "timer"
			//gettimeofday(&timestamp, NULL);
			timestamp = start.tv_sec * 1000 + start.tv_usec / 1000;
			printf("\ntimestamp: %ld\nbytes: %ld", timestamp, sizeof(timestamp));
			if (seqNum < MAXSEQNUM) {
				seqNum++;
			}
			else {
				seqNum = 1;
			}
			printf("\nseqnum = %u\nsize = %ld\n", seqNum, sizeof(seqNum));
			msgLength = 2 + sizeof(seqNum) + sizeof(timestamp) + strlen(sendline);
			printf("\nmsgLength = %d\nsize = %ld\n", msgLength, sizeof(msgLength));
			if (sendto(socketfd, (char *) sendline, strlen(sendline), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
				perror("Problem in sending the packet.");
				exit(4);
			}
			servlen = sizeof(servaddr); 
			if ((n = recvfrom(socketfd, (char *) recvline, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &servlen)) < 0) {
				perror("Problem in receiving the packet.");
				exit(5);
			}
			gettimeofday(&end, NULL); // End "timer"
			rtt = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000); // Calculate RTT	
			recvline[n] = '\0';
			printf("%s\n", recvline);
			printf("Round Trip Time (RTT) = %d(ms)\n", rtt);
			close(socketfd);
		}
	} while (1);

	return 0;
}
