#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Error function used for reporting issues
void error(const char *msg) {
  	perror(msg);
  	exit(1);
} 

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber){
 
  	// Clear out the address struct
  	memset((char*) address, '\0', sizeof(*address)); 

  	// The address should be network capable
  	address->sin_family = AF_INET;
  	// Store the port number
  	address->sin_port = htons(portNumber);
  	// Allow a client at any address to connect to this server
  	address->sin_addr.s_addr = INADDR_ANY;
}

int main(int argc, char *argv[]){
  	int connectionSocket, charsRead;
  	char buffer[256];
	char* line; //the buffer to hold each line in a file
	size_t lineSize = 0; //the buffer that represents the size of the line
	char* message; //the message that's being encrypted
	char* key; //the key that's being used to encrypt
  	struct sockaddr_in serverAddress, clientAddress;
  	socklen_t sizeOfClientInfo = sizeof(clientAddress);

  	// Check usage & args
  	if (argc < 2) { 
  		  fprintf(stderr,"USAGE: %s port\n", argv[0]); 
  		  exit(1);
  	} 
  
  	// Create the socket that will listen for connections
  	int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  	if (listenSocket < 0) {
  		  error("ERROR opening socket");
  	}

  	// Set up the address struct for the server socket
  	setupAddressStruct(&serverAddress, atoi(argv[1]));

  	// Associate the socket to the port
  	if (bind(listenSocket, 
          (struct sockaddr *)&serverAddress, 
          sizeof(serverAddress)) < 0){
  		  error("ERROR on binding");
  	  }

  	// Start listening for connetions. Allow up to 5 connections to queue up
  	listen(listenSocket, 5); 
  
  	// Accept a connection, blocking if one is not available until one connects
  	while(1){
    		// Accept the connection request which creates a connection socket
    		connectionSocket = accept(listenSocket, 
                (struct sockaddr *)&clientAddress, 
                &sizeOfClientInfo); 
    		if (connectionSocket < 0){
      			error("ERROR on accept");
    		}
    		printf("SERVER: Connected to client running at host %d port %d\n", 
                          ntohs(clientAddress.sin_addr.s_addr),
                          ntohs(clientAddress.sin_port));
		int i = 0;
		for (i; i < 2; i++) {
    			// Get the message from the client and display it
    			memset(buffer, '\0', 256);
    			// Read the client's message from the socket
    			charsRead = recv(connectionSocket, buffer, 255, 0); 
    			if (charsRead < 0){
      				error("ERROR reading from socket");
    			}
    			printf("SERVER: I received this from the client: \"%s\"\n", buffer);
    			//will need to write some code to read the files and store them
    			/*FILE* file = fopen(buffer, "r"); //open the file
			if (file == NULL) {
				//fclose(file);
				error("ERROR unable to open file\n");		
			}
			//read all the text into a given location
			else {
				write(1, "File ", 5);
				write(1, buffer, strlen(buffer));
				write(1, " opened successfully\n", 23);
				//printf("File %s opened successfully", buffer);*/
				/*while (getline(&line, &lineSize, file) != -1) {
					//do something
					strcat(message, line);
					//printf("%s\n", message);
				}*/
				/*if (i == 0) {if (getline(&message, &lineSize, file) == -1) {
					error("ERROR reading file\n");
				}*/
				//printf("%s\n", message);}
				/*lineSize = 0; 
			}
			fclose(file);*/	
		}		
    		// Send a Success message back to the client
    		charsRead = send(connectionSocket, 
                    "I am the server, and I got your message", 39, 0); 
    		if (charsRead < 0){
      			error("ERROR writing to socket");
    		}
    		// Close the connection socket for this client
    		close(connectionSocket); 
  	}
  	// Close the listening socket
  	close(listenSocket); 
  	return 0;
}
