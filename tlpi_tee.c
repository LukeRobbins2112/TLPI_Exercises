#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

void errExit(char * output){
  perror(output);
  _exit(1);
}


int main(int argc, char ** argv){

  if (argc < 2)
    printf("err - need file name\n");

  // Open file for output
  int flags = O_WRONLY | O_CREAT | O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = open(argv[1], flags, mode);

  
  // Read from stdin while not EOF
  // Continuously call read()
  
  int bytesRead = 0;
  char buf[BUF_SIZE];

  while((bytesRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0){

    printf("Bytes read: %d\n", bytesRead);
    
    // Null-terminate string
    buf[bytesRead] = '\0';

    if (strcmp(buf, "\n") == 0 ) break;

    // Output to both STDOUT and the file
    int bytesWritten = write(fd, buf, bytesRead + 1);
    printf("Bytes written: %d\n", bytesWritten);

    
    printf("%s", buf);

  }

  // Don't have to close STDIN fd
  
  return 0;

}
