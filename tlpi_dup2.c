#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

int main(){

  int specificFD = dup2(STDOUT_FILENO, 5);
  printf("File descriptor is: %d\n", specificFD);

  char buffer[BUF_SIZE];
  int bytesRead = 0;
  
  while((bytesRead = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0){

    // Null-terminate string
    buffer[bytesRead] = '\0';

    if (strcmp(buffer, "\n") == 0) break;

    write(specificFD, buffer, bytesRead);
  }


  return 0;

}
