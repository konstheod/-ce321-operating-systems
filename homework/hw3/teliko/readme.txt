asmlinkage long sys_slob_get_mem(int flag); ayto tha mpei sthn bibliothiki syscalls.h
ola ta alla bhmata gia thn eisagogh toy syscall einai ta idia

compile 
gcc -c my_sys.c -o my_sys.o

ar rcs libmy_sys_library.a my_sys.o

gcc -g -Wall test.c -o test -L. -lmy_sys_library