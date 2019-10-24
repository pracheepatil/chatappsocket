#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>

#define PORT "3400"

void error(char* msg){
	perror(msg);	
	exit(1);
}

int main(int argc, char *argv[]){
	if (argc != 3) {
		fprintf(stderr, "ERROR: usage: %s domain port", argv[0]);
		return -1;
	}
	
	int sockfd, n;
	struct addrinfo servaddr, *servinfo, *dup;
	int result, con_res;
	char buffer[255];
	memset(&servaddr, 0, sizeof servaddr);
	servaddr.ai_family = AF_INET;
	servaddr.ai_socktype = SOCK_STREAM;
	servaddr.ai_flags = AI_PASSIVE;
	result = getaddrinfo(argv[1], argv[2], &servaddr, &servinfo);
	if(result < 0 ){
		error("errorr......");
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("unable to create socket");
	}
	con_res = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	if(con_res == -1){
		error("Unable to connect");
	}
	
	while(1){
		memset(buffer, 0, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		if(n < 0){
			error("Error on sending");
		}
		
		memset(buffer, 0, 255);
		n = read(sockfd, buffer,255);
		if(n < 0){
		error("error on receving");
		}
		printf("Server msg: %s", buffer);

		int i = strncmp("bye", buffer, 3);
		if(i == 0)
		break;
	}

	return 0;
}
