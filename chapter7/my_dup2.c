#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define TARGET_FD 7

int myDup2(int originalFd, int newFD){

  if (originalFd == newFD){

    // Get info about the current open file
    int result = fcntl(originalFd, F_GETFL);

    // If not valid original fd
    if (result == -1){
      errno = EBADF;
      return -1;
    }

    // No need to duplicate, return original
    return originalFd;
  }

  // Get first FD, starting at target FD
  int dup2FD = fcntl(originalFd, F_DUPFD, newFD);

  // If result doesn't equal that, then newFD is already in use
  if (dup2FD != newFD){

    // Close it to make it available
    close(newFD);

    // Then re-call fcntl now that the fd is available
    dup2FD = fcntl(originalFd, F_DUPFD, newFD);

    if (dup2FD != newFD) perror("fcntl");

  }

  return dup2FD;

}

int main(int argc, char ** argv){

  if (argc < 3){
    printf("Error, need original and target fds\n");
    exit(1);
  }

  int origFD = atoi(argv[1]);     // FD to duplicate
  int targetFD = atoi(argv[2]);   // Desired FD value

  char buffer[BUF_SIZE];
  int bytesRead = 0;

  // Use target FD to test that dup2 takes it over
  int takeFD = dup2(origFD, targetFD);

  int copyFD = myDup2(origFD, targetFD);
  if (copyFD == -1) perror("dup2");

  printf("New fd from dup2(%d, %d): %d\n", origFD, targetFD, copyFD);

  while((bytesRead = read(STDIN_FILENO, buffer, BUF_SIZE)) > 0){

    // Null-terminate string
    buffer[bytesRead] = '\0';

    if (strcmp(buffer, "\n") == 0) break;

    write(copyFD, buffer, bytesRead);
  }


  return 0;

}
