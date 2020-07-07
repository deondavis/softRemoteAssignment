#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <unistd.h>
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

// Function designed to print the data from client and sent a response
void server_print(int sockfd) 
{ 
	char buff[MAX]; 
	// infinite loop for printing message from client 
	for (;;) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("From client: %s\n", buff); 
		// if msg contains "Exit" then server exit and chat ended.
                if (strncmp("exit", buff, 4) == 0) {
			bzero(buff, MAX);
                	// copy exit in the buffer
                	strncpy(buff,"exit",MAX);
                	// and send that buffer to client
                	write(sockfd, buff, sizeof(buff));
                        printf("Server Exit...\n");
                        break;
		}
		else {
			bzero(buff, MAX);
                	// copy server message in the buffer
			strncpy(buff,"print SUCCESS",MAX);
			// and send that buffer to client 
			write(sockfd, buff, sizeof(buff)); 
		} 
	} 
} 

// Driver function 
int main(int argc, char* argv[]) 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 
	uint16_t port_number;
	port_number = (argc > 1) ? atoi(argv[1]):PORT;

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else {
		printf("Socket successfully created..\n"); 
	}
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(port_number); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else {
		printf("Server listening..\n"); 
	}
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else {
		printf("server acccept the client...\n"); 
	}

	// Function for printing client message
	server_print(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 

