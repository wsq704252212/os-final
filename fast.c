#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdatomic.h>

long int blockNumber;
// thread safe 
atomic_llong blockCnt = 0;

typedef struct {
    long int blocksize;
    char* filename;     
    unsigned int xorResult; 
} ThreadArg;

// xor
unsigned int xorbuf(unsigned int *buffer, long int size) {
    unsigned int result = 0;
    for (long int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

void *readBlock(void *arg) {
    ThreadArg *targ = (ThreadArg *)arg;
    long int n;
    long int bufsize = targ->blocksize / 4;

    unsigned int *buf = malloc(bufsize * sizeof(unsigned int));
    if (!buf) {
        perror("Memory allocation failed");
        pthread_exit(NULL);
    }

    int fd = open(targ->filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file in thread");
        free(buf);
        pthread_exit(NULL);
    }

    long int start;
    unsigned int threadXor = 0;
    for(;;) {
        // threads fetch blocks in order
        start = blockCnt++;
        if (start >= blockNumber) {
            break;
        }
        lseek(fd, start * targ->blocksize, SEEK_SET);
        n = read(fd, buf, targ->blocksize);

        long int size = n / sizeof(unsigned int);
        // if file size is not multiple of 4
        if (n % sizeof(unsigned int) != 0) {
            int byteCnt = sizeof(unsigned int) - n % sizeof(unsigned int);
            char *cbuf = (char*)buf;
            for (int i = 0; i < byteCnt; i++) {
                cbuf[n+i] = 0;
            }
            size = size + 1;
        }
        
        threadXor ^= xorbuf(buf, size);
    }

    targ->xorResult = threadXor;
    close(fd);
    free(buf);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int threadsNum, blockSize;
    char *file;

    // TODO argc == 2, use default
    if (argc == 2) {
        threadsNum = 32;
        blockSize = 4096;
    } else if (argc == 4) {
        threadsNum = atoi(argv[2]);
        blockSize = atoi(argv[3]);
    } else {
        fprintf(stderr, "Usage: fast <filename> [<number_of_threads> <block_size>]\n");
        exit(EXIT_FAILURE);
    }
    
    file = argv[1];
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    close(fd);

    blockNumber = fileSize / blockSize;
    if (fileSize % blockSize != 0) blockNumber++;

    pthread_t threads[threadsNum];
    ThreadArg args[threadsNum];


    for (int i = 0; i < threadsNum; i++) {
        args[i].filename = file;
        args[i].blocksize = blockSize;
        args[i].xorResult = 0;
    
        //printf("thread %d runing...\n", i);
        if (pthread_create(&threads[i], NULL, readBlock, &args[i])) {
            perror("Failed to create thread");
            for (int j = 0; j < i; j++) {
                pthread_cancel(threads[j]);
            }
            exit(EXIT_FAILURE);
        }
    }

    unsigned int totalXor = 0;
    for (int i = 0; i < threadsNum; i++) {
        pthread_join(threads[i], NULL);
        totalXor ^= args[i].xorResult;
    }

    printf("Total XOR: %08x\n", totalXor);

    return 0;
}
