#include <stdio.h>
#include "my_sys_library.h"

int main(int argc, char *argv[]){
  unsigned long int k;
  k = get_mem(0);
  printf("Flag: 0 total_free result: %lu\n",k);
  k = get_mem(1);
  printf("Flag: 1 total_allloc result: %lu\n",k);
 return(0); 
}