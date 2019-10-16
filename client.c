
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

void error(const char* msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

    if(argc < 3){
        error("invalid params.. example: ./client localhost 80");
    }
    int sockfd, n, portno;
    struct sockaddr_in servaddr, cli_addr;
    socklen_t clilen;
    char buffer[255];
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("unable to create socket");
    }
    portno = atoi(argv[2]);
    server = gethostbyname(argv[1]);
    if(server == NULL){
      fprintf(stderr, "Error, No such host");
    }

    bzero((char*)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &servaddr.sin_addr.s_addr, server->h_length);
     
    servaddr.sin_port = htons(portno);

    n = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(n <0){
      error("Connection failed");
    }

    while(1)
  {
    bzero(buffer, 255);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if(n<0){
      error("Error on sending");
    }
    bzero(buffer, 255);
    n = read(sockfd, buffer,255);
    if(n<0){
      error("error on receving");
    }
    printf("Server msg: %s", buffer);

    int i = strncmp("bye", buffer, 3);
    if(i == 0)
      break;
  }
         
close(sockfd);
     return 0;

}
