#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv){

  // [program] [filename] [command]
  if (argc < 3){
    printf("Not enough args\n");
    _exit(1);
  }

  // Open the file
  int flags = O_RDWR | O_CREAT;
  mode_t mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = open(argv[1], flags, mode);

  if (fd == -1){
    perror("open");
    _exit(1);
  }

  printf("Opened file %s\n", argv[1]);

  // Evaluate the commands (loop through list given)

  for (int i = 2; i < argc; i++){

    int bytes = 0;
    off_t offset = 0;
    char * buf;
    
    switch(argv[i][0]){

    case 'r':

      bytes = atoi(&argv[i][1]);
      buf = (char *)malloc(bytes+1);
      
      int bytesRead = read(fd, buf, bytes);
      buf[bytesRead] = '\0';
      printf("Data read from file: %s\n", buf);
      free(buf);
      
      break;
    case 'w':

      bytes = atoi(&argv[i][1]);
      int len = strlen(&argv[i][1]);
      int bytesWritten = write(fd, &argv[i][1], len);

      if (bytesWritten == -1){
	perror("write");
	_exit(1);
      }
      printf("%s: Write succeeded\n", argv[i]);
      
      break;

    case 's':

      offset = (off_t)atoi(&argv[i][1]);
      int result = lseek(fd, offset, SEEK_SET);

      if (result == -1){
	perror("lseek");
	_exit(1);
      }
      printf("%s: seek succeeded\n", argv[i]);
      
      break;
      
    default:
      printf("default\n");
      break;

    }

  }
  



  // Close the file
  if (close(fd) == -1){
    perror("close");
    _exit(-1);
  }

}
