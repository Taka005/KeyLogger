#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "http.h"

#define BUFFER_SIZE 1024

void* request(char *method,char *hostname,char *port,char *path,char *type,char *data){
  int sock;
  int bytes;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[BUFFER_SIZE];
  char response[];

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

  sprintf(buffer,"%s HTTP/1.1\r\nHost: %s:%s\r\n",method,path,hostname);

  if(type != NULL&&data != NULL){
    sprintf(buffer + strlen(buffer),"Content-Type: %s\r\nContent-Length: %zu\r\n\r\n%s",type,strlen(data),data);
  }

  bytes = write(sock,buffer,strlen(buffer));
  if(bytes < 0){
    printf("ERROR: Can't writing to socket\n");
    close(sock);
    return NULL;
  }

  while(1){
    int read_size = read(sock,response,BUFFER_SIZE);
    printf("%d",read_size)
    if(read_size > 0){
      write(1,response,read_size);
    }else{
      break;
    }
  }

  close(sock);
  return strdup(response);

  memset(buffer,0,sizeof(buffer));
  bytes = read(sock,buffer,sizeof(buffer) - 1);
  if(bytes < 0){
    printf("ERROR: Can't reading from socket\n");
    return NULL;
  }

  printf("Connecting OK\n");

  close(sock);

  return strdup(buffer);
}

char* http_get(char *hostname,char *port,char *path){
  if(port == NULL){
    port = "80";
  }

  return request(
    "GET",
    hostname,
    port,
    path,
    NULL,
    NULL
  );
}

char* http_post(char *hostname,char *port,char *path,char *type,char *data){
  if(port == NULL){
    port = "80";
  }

  return request(
    "POST",
    hostname,
    port,
    path,
    type,
    data
  );
}