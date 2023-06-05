#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/

// Error function used for reporting issues
void error(const char *msg) { 
	perror(msg); 
  	exit(0); 
} 

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber){
 
  	// Clear out the address struct
  	memset((char*) address, '\0', sizeof(*address)); 

  	// The address should be network capable
  	address->sin_family = AF_INET;
  	// Store the port number
  	address->sin_port = htons(portNumber);

  	// Get the DNS entry for this host name
  	struct hostent* hostInfo = gethostbyname("localhost"); 
  	if (hostInfo == NULL) { 
  		  fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
  		  exit(0); 
  	}
  	// Copy the first IP address from the DNS entry to sin_addr.s_addr
  	memcpy((char*) &address->sin_addr.s_addr, 
 	      hostInfo->h_addr_list[0],
  	      hostInfo->h_length);
}

//count the characters in a file
//had ChatGPT make a program for me and then copied it into here
int count_characters(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("File not found.\n");
	printf("%s\n", file_path);
        return -1;
    }

    int character_count = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        character_count++;
    }

    fclose(file);
    return character_count;
}

int main(int argc, char *argv[]) {
  	int socketFD, portNumber, charsWritten, charsRead;
  	struct sockaddr_in serverAddress;
  	//char buffer[256];
  	// Check usage & args
  	if (argc != 4) { 
  		  fprintf(stderr,"USAGE: %s plaintext key port\n", argv[0]); 
  		  exit(0); 
  	}

	//store file names in varaibles
	char plaintext[strlen(argv[1])];
	strcpy(plaintext, argv[1]);
	char key[strlen(argv[2])];
	strcpy(key, argv[2]); 

  	// Create a socket
  	socketFD = socket(AF_INET, SOCK_STREAM, 0); 
  		if (socketFD < 0){
    		error("CLIENT: ERROR opening socket");
  	}

  	 // Set up the server address struct
  	setupAddressStruct(&serverAddress, atoi(argv[3]));

  	// Connect to server
  	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
    		error("CLIENT: ERROR connecting");
  	}
  	
	//get the amount of characters in the plaintext file
	int plaintextCount = count_characters(plaintext);
	if (plaintextCount == -1) {
		exit(1);
	}
	//get the amount of characters in the key file
	int keylen = count_characters(key);
	if (keylen == -1) {
		exit(1);
	}
	//make sure the key isn't shorter than the plaintext
	if (keylen < plaintextCount) {
		error("CLIENT: ERROR: key is shorter than plaintext\n");
	}
	
  	// Send message to server
  	// Write to the server
  	charsWritten = send(socketFD, plaintext, plaintextCount, 0); 
  	if (charsWritten < 0){
  		  error("CLIENT: ERROR writing to socket");
  	}
  	else if (charsWritten < plaintextCount){
    		printf("CLIENT: WARNING: Not all data written to socket!\n");
  	}
	else {
		printf("CLIENT: file %s sent with no errors or warnings.\n", argv[1]);
	}

	//now do the same thing except with the key
	charsWritten = send(socketFD, key, keylen, 0);
        if (charsWritten < 0){
                  error("CLIENT: ERROR writing to socket");
        }
        else if (charsWritten < plaintextCount){
                printf("CLIENT: WARNING: Not all data written to socket!\n");
        }
	else {
                printf("CLIENT: file %s sent with no errors or warnings.\n", argv[2]);
        }	


  	// Get return message from server
  	// Clear out the buffer again for reuse
  	/*memset(buffer, '\0', sizeof(buffer));
  	// Read data from the socket, leaving \0 at end
  	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); 
  	if (charsRead < 0){
    		error("CLIENT: ERROR reading from socket");
  	}*/
  	//printf("CLIENT: I received this from the server: \"%s\"\n", buffer);

  	// Close the socket
  	close(socketFD); 
  	return 0;
}
