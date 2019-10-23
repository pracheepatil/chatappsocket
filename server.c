#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT "3498"

void error(char* msg){
	perror(msg);
	exit(1);
}

int main()
{


int sockfd, result,newsockfd, n, bind_res;
char buffer[255];
struct addrinfo servaddr, *servinfo, *dup;
struct sockaddr_storage connector_addr;

memset(&servaddr, 0, sizeof(servaddr));

servaddr.ai_family = AF_INET;
servaddr.ai_socktype = SOCK_STREAM;
servaddr.ai_flags = AI_PASSIVE;



result = getaddrinfo(NULL, PORT, &servaddr, &servinfo);
if (result != 0){
	printf("error");
    return 1;
	
}
//else{
	//printf("error");
//	return -1;
//}
//printf("%d: ", result);
   sockfd = socket(dup->ai_family, dup->ai_socktype, 0);
   if(sockfd < 0){
	 error("unable to create socket");
   }

   printf("hello %d", sockfd);

   bind_res = bind(sockfd, dup->ai_addr, dup->ai_addrlen);
   if (bind_res < 0){
       error("unable to bind");
   }
   if(listen(sockfd, 5) < 0){
	   error("unable to listen");
   }
   
   int conn_addr_len = sizeof(connector_addr);
   
   newsockfd = accept(sockfd, (struct sockaddr*)&connector_addr, &conn_addr_len);
   if(newsockfd < 0 ){
	   error("unable to accept");
   }
   
  while(1){
	        memset(buffer, 0, 255);
            n = read(newsockfd, buffer, 255);
            if(n < 0){
                 error("error on receiving");
            }
            printf(" client msg : %s\n",buffer);
			
            memset(buffer, 0, 255);
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