#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "http.h"

#define BUFFER_SIZE 1024

char* request(char *method,char *hostname,char *port,char *path,char *type,char *data){
  int sock;
  int bytes;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char req[BUFFER_SIZE];
  char res[BUFFER_SIZE];

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

  sprintf(req,"%s %s HTTP/1.1\r\nHost: %s:%s\r\n",method,path,hostname,port);

  if(type != NULL&&data != NULL){
    sprintf(req + strlen(req),"Content-Type: %s\r\nContent-Length: %zu\r\n\r\n%s",type,strlen(data),data);
  }else{
    sprintf(req + strlen(req),"\r\n");
  }

  write(sock,req,strlen(req));

  while(1){
    int readByte = read(sock,res,BUFFER_SIZE);

    if(readByte > 0){
      write(1,res,readByte);
    }else{
      break;
    }
  }

  close(sock);

  return strdup(res);
}