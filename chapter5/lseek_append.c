#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 1024

int main(){

  int flags = O_RDWR | O_CREAT | O_APPEND;
  mode_t mode = S_IRWXU;
  int fd = open("file.txt", flags, mode);
  if (fd == -1) perror("open");

  char buffer[BUF_SIZE];
  int bytesRead = 0;

  while((bytesRead = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0){

    // Null-terminate string
    buffer[bytesRead] = '\0';

    if (strcmp(buffer, "\n") == 0) break;

    int status = lseek(fd, 0, SEEK_SET);

    write(fd, buffer, bytesRead);
  }


  return 0;

}
