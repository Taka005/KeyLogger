#include <stdio.h>
#include "http.h"

char *data = http_get("google.com",NULL,"/");

printf(data);