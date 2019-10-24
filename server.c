#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

void error(char* msg){
	perror(msg);
	exit(1);
}

int main()
{

  int sockfd, result,newsockfd,n;
  char buffer[255];
  struct addrinfo servaddr, *servinfo , *dup;
  struct sockaddr_storage connector_addr;

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.ai_family = AF_INET;
  servaddr.ai_socktype = SOCK_STREAM;
  servaddr.ai_flags = AI_PASSIVE;

  result = getaddrinfo("localhost", "3400", &servaddr, &servinfo);
  if (result != 0){
	printf("error");   
  }
for(dup = servinfo; dup != NULL; dup = dup->ai_next){
   sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
   if(sockfd < 0){
	 error("unable to create socket");
   }

   if(bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1){
     error("unable to bind");
   }
   break;
}
  
  freeaddrinfo(dup);
  if(dup == NULL){
      error("return ");
  }
  
if(listen(sockfd, 5) == -1)
   {
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