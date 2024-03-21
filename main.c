#include <stdio.h>
#include "http.h"

int main(){
  char *data = http_get("google.com",NULL,"/");

  if(data == NULL){
    return 1;
  }

  printf("%s",data);

  return 0;
}