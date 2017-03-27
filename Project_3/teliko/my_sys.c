#include<sys/syscall.h>
#include<unistd.h>
#include "my_sys_library.h"

/*me flag=0 emfanizei to total_free me flag=1 emfanizei to total_alloc 
 * me opoiodhpote allo emfanizei lathos sto dmesg kai gyrnaei -1*/
unsigned long int get_mem(int flag){
	 return(syscall(__NR_slob_get_mem,flag)); 
	
}