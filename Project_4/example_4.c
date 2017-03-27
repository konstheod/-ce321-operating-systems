#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define D 4000000
#define N 700000
// #define first_time
#define second_time

int main(int argc, char* argv[]) {
		
	long long int i = 0;
	int j;
	char buffer[N];
	
	int fd1 = open("ex1.txt", O_WRONLY | O_APPEND | O_CREAT);
	int fd2 = open("ex2.txt", O_WRONLY | O_APPEND | O_CREAT);
	int fd3 = open("ex3.txt", O_WRONLY | O_APPEND | O_CREAT);
	int fd4 = open("ex4.txt", O_WRONLY | O_APPEND | O_CREAT);
	int fd5 = open("ex5.txt", O_WRONLY | O_APPEND | O_CREAT);
#ifdef first_time	
	while(i<D) {
		write(fd1,"This will be output to testfile.txt\n", 36);
		write(fd2,"This will be output to testfile.txt\n", 36);
		write(fd3,"This will be output to testfile.txt\n", 36);
		write(fd4,"This will be output to testfile.txt\n", 36);
		write(fd5,"This will be output to testfile.txt\n", 36);
		i++;
	}
#endif
	close(fd1);
	close(fd5);
	close(fd4);
	close(fd3);
	
	fd1 = open("ex1.txt", O_WRONLY | O_APPEND | O_CREAT);
	fd5 = open("ex5.txt", O_WRONLY | O_APPEND | O_CREAT);
	
	write(fd1,"This will be output to testfile.txt\n", 36);
	
	fd3 = open("ex3.txt", O_WRONLY | O_APPEND | O_CREAT);
	read(fd5,&buffer,50*sizeof(char));
	
	close(fd1);
	fd4 = open("ex4.txt", O_WRONLY | O_APPEND | O_CREAT);
	write(fd4,"This will be output to testfile.txt\n", 36);
	lseek(fd4,SEEK_SET,0);
	read(fd4,&buffer,50*sizeof(char));
	
	write(fd2,"This will be output to testfile.txt\n", 36);
	
	read(fd3,&buffer,50*sizeof(char));
	write(fd3,"This will be output to testfile.txt\n", 36);
	lseek(fd5,SEEK_SET,0);
	read(fd5,&buffer,50*sizeof(char));
	write(fd3,"This will be output to testfile.txt\n", 36);
	
	j = 0;
	
	lseek(fd5,SEEK_SET,0);
	lseek(fd2,SEEK_SET,0);
	while(j<10000){
		read(fd5,&buffer,13000*sizeof(char));
		read(fd2,&buffer,13000*sizeof(char));
		j++;
	}
	
	close(fd2);
	close(fd5);
	close(fd4);
	close(fd3);
	
	fd2 = open("ex2.txt", O_WRONLY | O_APPEND | O_CREAT);
	fd3 = open("ex3.txt", O_WRONLY | O_APPEND | O_CREAT);
	fd4 = open("ex4.txt", O_WRONLY | O_APPEND | O_CREAT);
	fd5 = open("ex5.txt", O_WRONLY | O_APPEND | O_CREAT);

#ifdef second_time
	i=0;
	while(i<100) {
		read(fd5,&buffer,N*sizeof(char));
		read(fd3,&buffer,N*sizeof(char));
		read(fd4,&buffer,N*sizeof(char));
		read(fd2,&buffer,N*sizeof(char));
		
		i++;
	}
#endif
	
	
// // 	close(fd1);
	close(fd2);
	close(fd5);
	close(fd4);
	close(fd3);
	
#ifdef second_time
// 	unlink("ex3.txt");
// 	unlink("ex1.txt");
// 	unlink("ex5.txt");
// 	unlink("ex2.txt");
// 	unlink("ex4.txt");
#endif	
	return (0);
}