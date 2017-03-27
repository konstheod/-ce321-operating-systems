#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "my_sys_library.h"

#define times 30

int main(int argc, char *argv[]){
	unsigned long int total_mem,free_mem;
	long long int *l[times];
	int *k[times],*p[times],*n[times],*j[times],*u[times],*t[times];
	int i;
	for(i=0;i<times;i++){
		
		k[i] = (int*)malloc(sizeof(int)*20);
		n[i] = (int*)malloc(sizeof(int)*200);
		p[i] = (int*)malloc(sizeof(int)*5);
		free(p[i]);
		
		l[i] = (long long int*)malloc(sizeof(long long int)*2000);
		
		free(n[i]);
		total_mem = get_mem(1);
		free_mem = get_mem(0);
		
		printf("total free memory: %lu\n",free_mem);
		printf("total alllocated memory: %lu\n",total_mem);
		
		j[i] = (int *)malloc(sizeof(int)*50);
		u[i] = (int*)malloc(sizeof(int)*5);
		t[i] = (int*)malloc(sizeof(int)*5);
		sleep(1);
	}
	
	for(i=0;i<times;i++){
		free(k[i]);
		free(l[i]);
		free(j[i]);
		free(t[i]);
		free(u[i]);
	}
	return(0); 
}