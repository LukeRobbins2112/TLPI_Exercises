#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define FD_MIN 10

int myDup(int originalFd){


  return fcntl(originalFd, F_DUPFD, 0);

}

int main(){

  char buffer[BUF_SIZE];
  int bytesRead = 0;

  int copyFD = myDup(STDOUT_FILENO);
  if (copyFD == -1) perror("dup");

  printf("New fd: %d\n", copyFD);

  while((bytesRead = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0){

    // Null-terminate string
    buffer[bytesRead] = '\0';

    if (strcmp(buffer, "\n") == 0) break;

    write(copyFD, buffer, bytesRead);
  }


  return 0;

}
