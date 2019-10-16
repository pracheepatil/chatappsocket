
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


void error(const char* msg){
    perror(msg);
    exit(1);

}

int main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(stderr, "Please provide port");
        exit(1);
    }

        int sockfd, newsockfd, portno, n;
        struct sockaddr_in servaddr, cli_addr;
        socklen_t clilen;
        char buffer[255];

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0){
            error("unable to open socket");
        }
        bzero((char*) &servaddr, sizeof(servaddr));
        portno = atoi(argv[1]);
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(portno);

        if(bind(sockfd, (struct sockaddr *) &servaddr , sizeof(servaddr)) < 0){
            error("binding failed"); 
        }
        
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        
        newsockfd = accept(sockfd , (struct sockaddr *) &cli_addr, &clilen);

        if(newsockfd < 0){
            error("error on accept");
        }

        while(1){
            bzero(buffer, 255);
            n = read(newsockfd, buffer, 255);
            if(n < 0){
                 error("error on receiving");
            }
            printf(" client msg : %s\n",buffer);
            bzero(buffer, 255);
            fgets(buffer, 255, stdin);

            n = write(newsockfd, buffer, strlen(buffer));
            if(n<0){
                error("error on sending");
            }
            int i = strncmp("bye", buffer, 3);
            if(i==0)
            break;

        }
  
  close(newsockfd);
  close(sockfd);

  return 0;
}
