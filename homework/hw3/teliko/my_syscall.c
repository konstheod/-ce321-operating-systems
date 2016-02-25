#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/my_best_fit.h>

SYSCALL_DEFINE1(slob_get_mem,int,flag){
	
  
	if(flag==0){
		return(total_free);
	}
	else if(flag==1){
		
		 return(total_alloc);
	}
	else{
		printk("\nYou pass wrong argument\n");
		return(-1);
	}
}