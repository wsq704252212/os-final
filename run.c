#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


void
readFile(int fd, int blocksize, int blockcnt) {
  int n, i;
  // int is 4 bytes
  int bufsize = blocksize / 4;
  int *buf = malloc(bufsize * sizeof(int));

  for (i = 0; i < blockcnt; i++) {
    n = read(fd, buf, blocksize);
    if(n == 0) {
      printf("reach the end of file\n");
      break;
    }
    
    if(n < 0){
      printf("read error\n");
      exit(0);
    }
    printf("successfully read block %d, %d bytes in block\n", i, n);
  }
}

void
writeFile(int fd, int blocksize, int blockcnt) {
  int n, i;

  // char is 1 byte
  int bufsize = blocksize;
  char *buf = malloc(bufsize * sizeof(char));
  memset(buf, 'f', bufsize * sizeof(char));

  for (i = 0; i < blockcnt; i++) {
    n = write(fd, buf, bufsize);
    
    if(n != bufsize) {
      printf("write error\n");
      exit(0);
    }

    printf("successfully write block %d\n", i);
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if(argc != 5){
    printf("run error: insufficient arguments. Please try run <filename> [-r|-w] <block_size> <block_count>");
    exit(0);
  }

  char *file = argv[1];
  int blockSize = atoi(argv[3]), blockCnt = atoi(argv[4]);
  char mode = argv[2][1];
  int rwmode = (mode == 'r') ?  O_RDONLY : O_WRONLY|O_CREAT|O_TRUNC;

  if((fd = open(file, rwmode, 0666)) < 0){
    printf("run: cannot open %s\n", file);
    exit(0);
  }

  if(mode == 'r') {
    readFile(fd, blockSize, blockCnt);
  } else if(mode == 'w'){
    writeFile(fd, blockSize, blockCnt);
  }
  
  close(fd);
  exit(0);
}
