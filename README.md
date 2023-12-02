# How to use

1. Basic
```
gcc run.c -o run
run <filename> <-w|-r> <block_size> <block_count>
``` 

2. Measurment
```
gcc run2.c -o run2
run2 <filename> <block_size>
``` 

3. Raw Perfomance
```
test-raw.sh <filename>
``` 
The wall time to read file in different size is output to raw-speed.