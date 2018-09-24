#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <unistd.h>

#define MAXLINE 4096
#define SERV_PORT 10010

void numToByteArray(int32_t num, int size, char *bArray);
void buildPacket(int32_t a, int32_t b, char c, char *packet);
void decodePacket(char *receivePacket);

int main(int argc, char **argv) {
	int sockfd;
	struct sockaddr_in servaddr;
	struct hostent *he;
	char c;
	int a;
	int b;
	char packet[9];
	char receivePacket[14];

/*    
	// a: 0-3, b: 4-7, c: 8
	char msg[9] = {(char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0, (char) 0};
*/
	if (argc != 2) {
        	perror("Usage: ./client12 <server hostname>");
        	exit(1);
	}
    

    	
	//printf("CCCC: %c", packet[0]);

	//printf(" \n%p", *msg);
    
	if ((he = gethostbyname(argv[1])) == NULL) {
		perror("Problem in resolving server IP Address.");
		exit(1);
	}
    	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        	perror("Problem in creating the socket.");
        	exit(2);
    	}

    	memset(&servaddr, 0, sizeof(servaddr));
    	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = *((struct in_addr *)he->h_addr);
    	//servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    	servaddr.sin_port = htons(SERV_PORT);

    	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        	perror("Problem in connecting to the server");
        	exit(3);
    	}
	
	printf("\nEnter first number(>= 0): ");
    	scanf(" %d", &a);
    	printf("\nEnter second number(>= 0): ");
    	scanf(" %d", &b);
    	printf("\nEnter operator: ");
    	scanf(" %c", &c);
	
	buildPacket(a, b, c, packet);

    	/*while (fgets(sendline, MAXLINE, stdin) != NULL) {
        	send(sockfd, sendline, strlen(sendline), 0);

        	if (recv(sockfd, recvline, MAXLINE, 0) == 0) {
            		perror("The server terminated prematurely");
            		exit(4);
        	}
        	printf("String received from the server: ");
		recvline[strlen(recvline)] = '\0';
        	fputs(recvline, stdout);
    	}*/
	send(sockfd, packet, 9, 0);
	recv(sockfd, receivePacket, 14, 0);
	
	decodePacket(receivePacket);
	
	close(sockfd);
    	return 0;
}


void numToByteArray(int32_t num, int size, char *bArray) {
	int i;
	/*for (i = size - 1; i >= 0; i--) {
		bArray[i] = (num >> (i * 8) & 0xFF);
	}*/
	/*for (i = 0; i < size; i++) {
		bArray[i] = (num >> ((size - 1 - i) * 8)) & 0xFF;
	}*/
	for (i = 0; i < size; i++) {
		bArray[i] = (num >> (i * 8)) & 0xFF;
	}
}

void buildPacket(int32_t a, int32_t b, char c, char *packet) {
	char partA[4];
	char partB[4];

	numToByteArray(a, sizeof(partA), partA);
	numToByteArray(b, sizeof(partB), partB);

	memcpy(packet + 1, partA, sizeof(partA));
	memcpy(packet + 1 + sizeof(partA), partB, sizeof(partB));
	packet[0] = c;
}

void decodePacket(char *receivePacket) {
	int32_t answer = 0;
	int i;

	for (i = 12; i >= 9; i--) {
		answer += receivePacket[i];
		if (i > 9) {
			answer = answer << 8;
		}
	}

	if (receivePacket[13] == '1') {
		// valid
		printf("\nAnswer is valid.\n");
		printf("Answer = %d\n", answer);
	}
	else {
		// invalid
		printf("\nAnswer is invalid.\n");
		printf("Answer = %d\n", answer);
	}

	
}
