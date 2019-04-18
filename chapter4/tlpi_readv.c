#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

int main(){


  // Create a new file for reading and writing
  int flags = O_RDWR | O_CREAT | O_APPEND | O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH | S_IWOTH;

  int fd = open("readv.txt", flags, mode);

  if (fd == -1){
    perror("open");
    _exit(1);
  }

  // Read in data from STDIN, write to file
  char buf[BUF_SIZE];
  int bytesRead = 0;
  while((bytesRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0){

    buf[bytesRead] = '\0';  // Error if buf read all 1024
    if (strcmp(buf, "\n") == 0) break;

    int bytesWritten = write(fd, buf, bytesRead);
    if (bytesWritten < bytesRead){
      perror("write");
      _exit(1);
    }
  }

  // Seek to beginning of the file, since readv() uses current file offset position
  off_t result = lseek(fd, 0, SEEK_SET);
  if (result == -1){
    perror("lseek");
    _exit(1);
  }

  // Scatter read operations
  char buf0[11];
  char buf1[6];
  char buf2[6];

  struct iovec iov[3];

  iov[0].iov_base = buf0;
  iov[0].iov_len = 10;
  iov[1].iov_base = buf1;
  iov[1].iov_len = 5;
  iov[2].iov_base = buf2;
  iov[2].iov_len = 5;


  ssize_t bytesReadV = readv(fd, iov, 3);
  if (bytesReadV == -1){
    perror("readv");
    _exit(1);
  }
  
  // Null terminate data for printing
  buf0[10] = '\0';
  buf1[5] = '\0';
  buf2[5] = '\0';
  
  printf("Bytes read: %li\n", (long int) bytesReadV);

  printf("Buf %d, %s\n", 0, buf0);
  printf("Buf %d, %s\n", 1, buf1);
  printf("Buf %d, %s\n", 2, buf2);


  return 0;

}
