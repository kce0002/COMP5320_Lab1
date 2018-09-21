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

#define SERV_PORT 10010
#define MAXLINE 1024
#define MAXDIGITS 5
#define NUM_AMOUNT 10000

int main(int argc, char **argv) {
	
	if (argc != 2) {
		perror("Usage: UDPClient <server hostname>");
		exit(1);
	}
	
	char numbers[NUM_AMOUNT + 1][MAXDIGITS + 1];
	char numbersReceived[NUM_AMOUNT + 1][MAXDIGITS + 1];
	int i;
	char temp[MAXDIGITS + 1];
	int socketfd;
	int n;
	socklen_t servlen;
	struct sockaddr_in servaddr;
	struct hostent *he;
	//int numSent = 0;
	//int numRcvd = 0;
	int pid;
	
	for (i = 1; i <= NUM_AMOUNT; i++) {
		sprintf(temp, "%d", i);
		strcpy(numbers[i - 1], temp);
	}
	numbers[NUM_AMOUNT][MAXDIGITS] = '\0';
	
	if ((he = gethostbyname(argv[1])) == NULL) {
		perror("gethostbyname");
		exit(1);
	}

	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Problem in creating the socket");
		exit(2);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_addr = *((struct in_addr *)he->h_addr);
	servaddr.sin_port = htons(SERV_PORT);

	pid = fork();
	
	// Child - Receive data
	if (pid == 0) {
		for (i = 0; i < NUM_AMOUNT; i++) {
		//i = 0;
		//while(i < NUM_AMOUNT) {
			servlen = sizeof(servaddr);	
			if ((n = recvfrom(socketfd, (char *) numbersReceived[i], MAXDIGITS, MSG_WAITALL,(struct sockaddr *) &servaddr, &servlen)) < 0) {
				perror("Error in recvfrom");
				exit(4);
			}
			//else {
				//i++;
			//	numRcvd++;
			//	printf("#Recvd: %d\n", numRcvd);
			//}
			//if (n >= MAXDIGITS) {
			//	printf("!!!!!!!!!!!!!!\n");
			//}
			//numbersReceived[NUM_AMOUNT][MAXDIGITS] = '\0';
			printf("Server: %s, index: %d\n", numbersReceived[i], i);
			// This is NOT a full proof method for ending the loop
			if (strcmp("10000\0", numbersReceived[i]) == 0) {
				break;
			}		
		}	
	}
	// Parent - Send data
	else {
		for (i = 0; i < NUM_AMOUNT; i++) {
			printf("Sending %s\n", numbers[i]);

			if (sendto(socketfd, (char *) numbers[i], strlen(numbers[i]), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
				perror("Error in sendto");
				exit(3);
			}
			/*else {
				numSent++;
				printf("#Sent: %d\n", numSent);
			}*/
		}


	}

	
//	for (i = 0; i < NUM_AMOUNT; i++) {
//		printf("Sending %s\n", numbers[i]);
//
//		if (sendto(socketfd, (char *) numbers[i], strlen(numbers[i]), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
//			perror("Error in sendto");
//			exit(3);
//		}
//		/*else {
//			numSent++;
//			printf("#Sent: %d\n", numSent);
//		}*/
//	}

	//i = 0;	
	// This will likely fail if 10000 doesnt get received
	// Currently stays as an infinite loop
	// Maybe do the recvfrom in the loop conditional statement
	/*while (strcmp("10000\0", numbersReceived[i]) != 0) {

	
		n = recvfrom(socketfd, (char *) numbersReceived[i], MAXDIGITS, MSG_WAITALL, (struct sockaddr *) &servaddr, &servlen);
		printf("Server: %s, index: %d\n", numbersReceived[i], i);
		i++;
	}*/

//	for (i = 0; i < NUM_AMOUNT; i++) {
//	//i = 0;
//	//while(i < NUM_AMOUNT) {
//		servlen = sizeof(servaddr);	
//		if ((n = recvfrom(socketfd, (char *) numbersReceived[i], MAXDIGITS, MSG_WAITALL,(struct sockaddr *) &servaddr, &servlen)) < 0) {
//			perror("Error in recvfrom");
//			exit(4);
//		}
//		/*else {
//			//i++;
//			numRcvd++;
//			printf("#Recvd: %d\n", numRcvd);
//		}*/
//		/*if (n >= MAXDIGITS) {
//			printf("!!!!!!!!!!!!!!\n");
//		}*/
//		//numbersReceived[NUM_AMOUNT][MAXDIGITS] = '\0';
//		printf("Server: %s, index: %d\n", numbersReceived[i], i);
//		// This is NOT a full proof method for ending the loop
//		if (strcmp("10000\0", numbersReceived[i]) == 0) {
//			break;
//		}		
//	}
	close(socketfd);


	return 0;
}
