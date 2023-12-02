#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>


int
readFile(int fd, int blocksize) {
  int n, i;
  // int is 4 bytes
  int bufsize = blocksize / 4;
  int *buf = malloc(bufsize * sizeof(int));
  int cnt = 0;

  while ((n = read(fd, buf, blocksize)) > 0) {
    cnt++;
  }

  if(n < 0){
    printf("read error\n");
    exit(0);
  }

  return cnt;
}

double now() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc != 3){
    printf("run error: not correct arguments. Please try run <filename> <block_size>");
    exit(0);
  }

  char *file = argv[1];
  int blockSize = atoi(argv[2]);

  if((fd = open(file, O_RDONLY, 0666)) < 0){
    printf("run: cannot open %s\n", file);
    exit(0);
  }
  
  double start = now();
  int cnt = readFile(fd, blockSize);
  double end = now();
  printf("Finished reading in %f seconds\n", end - start);  
  printf("block_count: %d\n", cnt);

  close(fd);

  exit(0);
}
