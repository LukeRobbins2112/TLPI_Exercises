#define _FILE_OFFSET_BITS 64
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

 int fd;
 off_t off;

 if (argc != 3 || strcmp(argv[1], "--help") == 0)
   printf("%s pathname offset\n", argv[0]);

 fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

 if (fd == -1)
   perror("open");

 off = atoll(argv[2]);

 if (lseek(fd, off, SEEK_SET) == -1)
   perror("lseek");

 if (write(fd, "test", 4) == -1)
   perror("write");

 _exit(EXIT_SUCCESS);
}
