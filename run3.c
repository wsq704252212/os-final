#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>

// return block cnt
unsigned long
lseekFile(int fd) {
  off_t n;
  off_t end = lseek(fd, 0, SEEK_END);

  unsigned long byteCnt = 0;
  
  lseek(fd, 0, SEEK_SET);
  while ((n = lseek(fd, 1, SEEK_CUR)) != end) {
    byteCnt++;
  }
  return byteCnt;
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

  if(argc != 2){
    printf("run error: not correct arguments. Please try run <filename>");
    exit(0);
  }

  char *file = argv[1];

  if((fd = open(file, O_RDONLY, 0666)) < 0){
    printf("run: cannot open %s\n", file);
    exit(0);
  }
  
  double start = now();
  unsigned long cnt = lseekFile(fd);
  double end = now();

  printf("lseek() system call number per second: %f B/s\n", cnt / (end - start)); 

  close(fd);

  exit(0);
}
