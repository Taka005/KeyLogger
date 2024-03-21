#include <stdio.h>
#include "http.h"

int main(){
  char* data = sendRequest("GET","google.com","80","/",NULL,NULL);

  if(data == NULL){
    return 1;
  }

  printf("%s",data);

  return 0;
}