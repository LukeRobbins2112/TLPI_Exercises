#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024

// Function declarations
char * userNameFromId(uid_t uid);
uid_t userIdFromName(const char *name);
void traverseProc(const char * dirPath, const char * nameArg);
void process_pid_status(const char * fileName, const char * UID);



/* Return name corresponding to 'uid', or NULL on error */
char * userNameFromId(uid_t uid)
{
  struct passwd *pwd;
  pwd = getpwuid(uid);
  return (pwd == NULL) ? NULL : pwd->pw_name;
}

/* Return UID corresponding to 'name', or -1 on error */
uid_t userIdFromName(const char *name)
{
  struct passwd *pwd;
  uid_t u;
  char *endptr;

  if (name == NULL || *name == '\0')
    return -1;

  u = strtol(name, &endptr, 10); /* As a convenience to caller */

  if (*endptr == '\0') /* allow a numeric string */
    return u;

  pwd = getpwnam(name);

  if (pwd == NULL)
    return -1;

  return pwd->pw_uid;
}


void traverseProc(const char * dirPath, const char * nameArg){

  DIR * dirPtr;
  struct dirent *dp;

  dirPtr = opendir(dirPath);
  if (dirPtr == NULL) {
    printf("opendir failed on '%s'", dirPath);
    return;
  }

  uid_t myUID = userIdFromName(nameArg);
  char stringUID[16];
  sprintf(stringUID, "%d", (int)myUID);
  printf("Searching for UID %s\n", stringUID);

  /* For each entry in this directory, print directory + filename */
  for (;;) {

    errno = 0; /* To distinguish error from end-of-directory */
    dp = readdir(dirPtr);

    if (dp == NULL)
      break;

    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
      continue; /* Skip . and .. */

    // Only process PID directories, which are numbers
    int is_PID_Dir = 1;
    size_t subDirLen = strlen(dp->d_name);
    for (size_t i = 0; i < subDirLen; i++){
      if (!isdigit(dp->d_name[i]))
        is_PID_Dir = 0;
    }

    if (!is_PID_Dir) continue;

    size_t fullLen = strlen(dirPath) + subDirLen + 2 + 7;    // + 2 for / and null terminator, + 7 for /status
    char * fullPath = (char *)malloc(fullLen);
    strcpy(fullPath, dirPath);
    strcat(fullPath, "/");
    strcat(fullPath, dp->d_name);
    strcat(fullPath, "/status");
    fullPath[fullLen] = '\0';
    // printf("Full Path: %s\n", fullPath);

    process_pid_status(fullPath, stringUID);
  }

  if (errno != 0)
    perror("readdir");

  if (closedir(dirPtr) == -1)
    perror("closedir");
}


void process_pid_status(const char * fileName, const char * UID){

  int flags = O_RDONLY;
  mode_t mode = S_IRWXU;
  int PID_fd = open(fileName, flags, mode);

  if (PID_fd == -1){
    perror("open");
    exit(1);
  }

  char buf[BUF_SIZE+1];
  int bytesRead = 0;
  while((bytesRead = read(PID_fd, buf, BUF_SIZE)) > 0){

    // Null-terminate
    buf[bytesRead] = '\0';

    if (strstr(buf, "Uid") != NULL && strstr(buf, UID) != NULL){
      // This is the Uid line, and the user we specified
      printf("Process %s belongs to user %s\n", fileName, UID);
      break;
    }

  }

  if (close(PID_fd) == -1){
    perror("close");
    exit(1);
  }

}

int main(int argc, char **argv){

  if (argc < 2){
    printf("err - need to supply user name\n");
    exit(1);
  }

  traverseProc("/proc", argv[1]);

  return 0;
}
