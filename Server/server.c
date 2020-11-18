// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

void handle_request(int new_socket) {

    int valread = 0;
    char filename[1024] = {0}; 
	char *hello = "Hello from server\n"; 
    
    valread = read( new_socket , filename, 1024); 
    filename[valread] = 0; // end what read
	printf("%s\n",filename ); // what the client says
	
	// read file 
    char* text;
    FILE *pf = fopen(filename,"r");
    fseek(pf,0,SEEK_END);
    long lSize = ftell(pf);
    // 用完后需要将内存free掉
    text=(char*)malloc(lSize+1);
    rewind(pf); 
    fread(text,sizeof(char),lSize,pf);
    text[lSize] = '\0';	
    printf("size is %lo,\n%s \n",lSize, text);
    
    
    char size[200];
    sprintf(size, "%lo" , lSize);
    
// 	send(new_socket , hello , strlen(hello) , 0 );
    write(new_socket, size, strlen(size));
    write(new_socket, "*", strlen("*"));
    write(new_socket, text, strlen(text));
	
	printf("\nServer: filesize + flag + content sent\n"); 
	free(text);
}


int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 

	short port;
	
	if(argc < 2){
	    printf("Usage: %s PORT\n", argv[0]);
	    exit(0);
	}
	
	port = (short)atoi(argv[1]);
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( port ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	printf("======waiting for client's request======\n");  
	while(1) {
	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
    	{ 
    		perror("accept"); 
    		exit(EXIT_FAILURE); 
    	} 
    	printf("New Connection\n");  
        
        handle_request(new_socket);	

    	
	}
	
	
	
	return 0; 
} 

