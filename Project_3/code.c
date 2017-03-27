#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#define N 10

struct slob{
  int units;
  int status;
  struct slob *next;
};


int main(int argc, char *argv[]){
	int counter,i,j,flag_if,min,prev_min;
	int size;
	
	int array[N][N];
	struct slob *sp,*curr,*prev,*root;
	int b;
	
	printf("Give the size for %d pages and the status\n",N);
	for(i=0;i<N;i++){
		array[i][0] = 0;
		array[i][1] = 0;
		scanf("%d %d",&array[i][0],&array[i][1]);
	}	
	
	
	for(i=0;i<N;i++){
		curr = (struct slob *)malloc(1*sizeof(struct slob));
		curr->units = array[i][0];
		curr->status = array[i][1];
		if(i==0){
			sp = curr;
			prev = curr;
		}
		else{
			prev->next = curr;
			prev = curr;
		}
	}
	
	printf("Give page request\n");
	scanf("%d",&size);
	
	curr = sp;
	for(i=0;i<N;i++){
		printf("units %d status %d\n",curr->units,curr->status);\
		curr = curr->next;
	}
	
	counter = 1;
	i = 0;
	flag_if = 0;
	min = 0;
	prev_min = size;
	root = sp;
	
	while(1){
		for(j=0;j<N;j++){
			/* Enough room on this page? */
			if (sp->units < prev_min)
				sp = sp->next;
				continue;
			
			if(flag_if==0){
				min = sp->units;
				curr = sp;
				flag_if = 1;
				sp = sp->next;
			}
			else{
				if(sp->units <= min){
					if(sp->units == min){
						if(i==0){
							counter++;
							curr = sp;
							min = sp->units;
						}
						else{
							i++;
						}
					}
					else{
						i = 0;
						counter = 1;
						curr = sp;
						min = sp->units;
					}
					if(i==counter){
						curr = sp;
						min = sp->units;
						break;
					}
				}
				sp = sp->next;
			}
		}
			
		//kaloyme thn slob_page_alloc
		counter--;
		if(curr!=NULL){
			b = curr->status;
		}
		else{
			b = 0;
			break;
		}
		if (b == 0){
			if(counter == 0) {
				flag_if = 0;
				i = 0;
				counter = 1;
				prev_min = min;
				curr = NULL;
			}
			sp = root;
			continue;
		}
		else{
			break;
		}
	}
	printf("b: %d status: size: \n",b/*,curr->status,curr->units*/);
	return (0);
}