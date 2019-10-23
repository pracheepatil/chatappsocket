#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT "3400"

void error(char* msg){
	perror(msg);
	exit(1);
}

int main(){
  int sockfd, result,newsockfd, n;
  char buffer[255];
  struct addrinfo servaddr, *servinfo, *dup;
  struct sockaddr_storage connector_addr;

  memset(&servaddr, 0, sizeof servaddr);
  servaddr.ai_family = AF_INET;
  servaddr.ai_socktype = SOCK_STREAM;
  servaddr.ai_flags = AI_PASSIVE;

  result = getaddrinfo(NULL, "3400", &servaddr, &servinfo);
  if (result != 0) {
    error("error in getaddrinfo");
  }
  
  /*
  * getaddrinfo returns a list of addresses, loop through it
  * Once you find a valid address, create socket
  * if no address available no need to create socket
  */

  for (dup = servinfo; dup != NULL; dup = dup->ai_next) {
    /* 
    * always avaoid duplication, don't use config MACROS everywhere. e.g. in socket()
    * it's already defined in servaddr struct, use that
    * Returned real addressess will also have that same config, u can use that too
    */
    sockfd = socket(dup->ai_family, dup->ai_socktype, dup->ai_protocol);
    if (sockfd < 0) {
      error("unable to create socket");
    }

    if (bind(sockfd, dup->ai_addr, dup->ai_addrlen) == -1) {
      error("unable to bind the socket");
    }
    // once socket binding is successfull, break the loop
    break;
  }
  
  // free servinfo, because we created the socket successfully
  freeaddrinfo(servinfo);
  if (dup == NULL) {
    error("Failed to bind");
  }

  if (listen(sockfd, 5) < 0) {
    error("unable to listen");
  }
  
  printf("listening for connections...\n");

  int conn_addr_len = sizeof(connector_addr);
   
  newsockfd = accept(sockfd, (struct sockaddr*)&connector_addr, &conn_addr_len);
  if(newsockfd <0 ){
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
