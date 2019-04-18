#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define FD_MIN 10

int main(){

  int firstFD = fcntl(STDOUT_FILENO, F_DUPFD, FD_MIN);
  int secondFD = fcntl(STDOUT_FILENO, F_DUPFD, FD_MIN);
  printf("First fd available from %d+: %d\n", FD_MIN, firstFD);
  printf("Second fd available from %d+: %d\n", FD_MIN, secondFD);

  char buffer[BUF_SIZE];
  int bytesRead = 0;
  
  while((bytesRead = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0){

    // Null-terminate string
    buffer[bytesRead] = '\0';

    if (strcmp(buffer, "\n") == 0) break;

    write(secondFD, buffer, bytesRead);
  }


  return 0;

}
