// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>

void* handle_request(void *new_socket_p) {
    int new_socket = (int)(long)new_socket_p;
// void handle_request(int new_socket) {
    
    int valread = 0;
    char filename[1024] = {0}; 
	char *hello = "Hello from server\n"; 
    
    valread = read( new_socket , filename, 1024); 
    
    filename[valread] = 0; // end what read
	printf("filename is %s\n",filename ); // what the client says
	
	char filePath[100]; // declare file path 
	strcpy(filePath, "/home/ubuntu/environment/Socket Programming C/Server/files/"); // set file path 
	const char * path = filePath;
	strcat(filePath, filename);
	printf("filePath is %s\n",filePath );
	//printf("hi");
	
	// read file 
    char* text;
    FILE *pf = fopen(path ,"r");
    fseek(pf,0,SEEK_END);
    long lSize = ftell(pf);
    // 用完后需要将内存free掉
    text=(char*)malloc(lSize+1);
    rewind(pf); 
    fread(text,sizeof(char),lSize,pf);
    text[lSize] = '\0';	
    // printf("size is %lo,\n%s \n",lSize, text);
    printf("size is %lo\n",lSize);
    
    
    char size[200];
    sprintf(size, "%lo" , lSize);
    
// 	send(new_socket , hello , strlen(hello) , 0 );
    write(new_socket, size, strlen(size));
    write(new_socket, "*", strlen("*"));
    write(new_socket, text, strlen(text));
	
	printf("Server: filesize + flag + content sent\n"); 
	free(text);
}


  
  pthread_t tid; //pthread_t ==> long

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
        //printf("%i\n", new_socket);  
        
        //handle_request(new_socket);	
        
        // int a = rand();
        // printf("Launching thread with id %d to handle incoming request\n",a);
        
        // we have to pass in a long pointer as argument...... it took me hours to figure out...
        if(pthread_create(&tid, NULL, handle_request, (void *)(long)new_socket) != 0){ 
             perror("Error while creating thread.");
        }

    	
	}
	
	return 0; 
} 

