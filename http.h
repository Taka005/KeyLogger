#ifndef HTTP_H
#define HTTP_H

void* request(char *method,char *hostname,char *port,char *path,char *type,char *data);

char* http_get(char *hostname,char *port,char *path);

char* http_post(char *hostname,char *port,char *path,char *type,char *data);

#endif