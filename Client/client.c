// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *hello = "Hello from client"; 
	char buffer[1024] = {0}; 
	short port;
	
	if(argc < 2){
	    printf("Usage: %s PORT\n", argv[0]);
	    exit(0);
	}
	
	port = (short)atoi(argv[1]);
	
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 


	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(port); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	
	char filename[200];
	printf("Enter the name of the file you want to look at: \n"); 
    fgets(filename, 200, stdin);  
    filename[strcspn(filename, "\n")] = 0; // get rid of the last new line
    printf("You enterrd: %s \n", filename); 
	
// 	send(sock , hello , strlen(hello) , 0 ); 
    write(sock, hello, strlen(hello));
	printf("Hello message sent\n"); 
	valread = read( sock , buffer, 1024); 
	printf("%s\n",buffer ); // what the server says 
	return 0; 
} 
