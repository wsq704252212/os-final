# How to use

1. Basic
```bash
gcc run.c -o run
run <filename> <-w|-r> <block_size> <block_count>
``` 

2. Measurment
```bash
gcc run2.c -o run2
run2 <filename> <block_size>
``` 

3. Raw Perfomance
```bash
test-raw.sh <filename>
``` 
The wall time to read file in different block size is output to raw-speed.

4. Cache
```bash
test-cache.sh <filename>
``` 
The wall time to read file in different block size without/with page cache is output to raw-noncache-speed/raw-cache-speed.


5. Syscall
```bash
test-syscall.sh <filename>
``` 
The wall time to make syscall is output to stdout.

6. Fast
```bash
gcc -O3 fast.c -o fast
fast <filename> <thread_number> <block_size>
# to use default configure
fast <filename>

# how to test
test-fast.sh <filename> <thread_number>
``` 
The wall time to read file in different block size is output to fast-realtime.

