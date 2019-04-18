#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_SIZE 1024

int main(int argc, char ** argv){

  if (argc != 3){
    printf("Err - incorrect num arguments\n");
    _exit(-1);
  }

  
  int inputFd, outputFd;
  int openFlags;
  mode_t filePermissions;

  char buffer[BUF_SIZE];

  // Open file for read - first arg
  inputFd = open(argv[1], O_RDONLY);
  if (inputFd == -1){
    printf("Err - unable to open file for reading\n");
    _exit(-1);
  }

  // Set flags and open second file

  // Create if doesn't exist, truncate if does, write only
  openFlags = O_CREAT | O_TRUNC | O_WRONLY;

  // -rw-rw-rw-
  filePermissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  outputFd = open(argv[2], openFlags, filePermissions);
  
  if (outputFd == -1){
    printf("Err - unable to open file for reading\n");
    _exit(-1);
  }

  size_t bytes;
  while((bytes = read(inputFd, buffer, BUF_SIZE)) > 0){
    if (write(outputFd, buffer, bytes) != bytes){
      printf("Err - not all bytes coud be written\n");
      _exit(-1);
    }
  }

  if (close(inputFd) == -1){
    printf("Err closing input file\n");
    _exit(-1);
  }

  if (close(outputFd) == -1){
    printf("Err closing output file\n");
    _exit(-1);
  }

  return 0;

}
