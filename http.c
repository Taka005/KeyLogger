#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "http.h"

#define BUFFER_SIZE 1024

char* sendRequest(char *method,char *hostname,char *port,char *path,char *type,char *data){
  int sock;
  int bytes;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char request[BUFFER_SIZE];
  char response[BUFFER_SIZE];

  sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock < 0){
    printf("ERROR: Can't opening socket\n");
    close(sock);
    return NULL;
  }

  server = gethostbyname(hostname);
  if(server == NULL){
    printf("ERROR: No such host\n");
    close(sock);
    return NULL;
  }

  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memcpy((char *)&serv_addr.sin_addr.s_addr,(char *)server->h_addr,server->h_length);
  serv_addr.sin_port = htons(atoi(port));

  if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
    printf("ERROR: Can't connecting\n");
    close(sock);
    return NULL;
  }

  sprintf(request,"%s %s HTTP/1.1\r\n",method,path);
  write(sock,request,strlen(request));

  sprintf(request,"Host: %s:%s\r\n",hostname,port);
  write(sock,request,strlen(request));

  if(type != NULL&&data != NULL){
    sprintf(request,"Content-Type: %s\r\nContent-Length: %zu\r\n\r\n%s",type,strlen(data),data);
    write(sock,request,strlen(request));
  }

  while(1){
    int readSize = read(sock,response,BUFFER_SIZE);

    if(readSize > 0){
      write(1,response,readSize);
    }else{
      break;
    }
  }

  return strdup(response);

  bytes = write(sock,request,strlen(request));
  if(bytes < 0){
    printf("ERROR: Can't writing to socket\n");
    close(sock);
    return NULL;
  }

  memset(request,0,sizeof(request));
  bytes = read(sock,request,sizeof(request) - 1);
  if(bytes < 0){
    printf("ERROR: Can't reading from socket\n");
    return NULL;
  }

  printf("Connecting OK\n");

  close(sock);

  return strdup(request);
}