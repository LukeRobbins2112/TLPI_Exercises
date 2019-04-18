
// #include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALLOCS 1000000

int main(int argc, char *argv[]){

  char *ptr[MAX_ALLOCS];
  int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

  printf("\n");

  if (argc < 3 || strcmp(argv[1], "--help") == 0)
    printf("%s num-allocs block-size [step [min [max]]]\n", argv[0]);

  numAllocs = atoi(argv[1]);

  if (numAllocs > MAX_ALLOCS)
    printf("num-allocs > %d\n", MAX_ALLOCS);

  blockSize = atoi(argv[2]);
  freeStep = (argc > 3) ? atoi(argv[3]) : 1;
  freeMin = (argc > 4) ? atoi(argv[4]) : 1;
  freeMax = (argc > 5) ? atoi(argv[5]) : numAllocs;

  if (freeMax > numAllocs)
    printf("free-max > num-allocs\n");

  printf("Initial program break: %10p\n", sbrk(0));
  printf("Allocating %d*%d bytes\n", numAllocs, blockSize);

  for (j = 0; j < numAllocs; j++) {
    ptr[j] = malloc(blockSize);
    if (ptr[j] == NULL)
       printf("malloc err");
    printf("Program break is now: %10p\n", sbrk(0));
  }

  printf("Final program break is: %10p\n", sbrk(0));
  printf("Freeing blocks from %d to %d in steps of %d\n",
         freeMin, freeMax, freeStep);

  for (j = freeMin - 1; j < freeMax; j += freeStep)
    free(ptr[j]);

  printf("After free(), program break is: %10p\n", sbrk(0));
  exit(EXIT_SUCCESS);
}
