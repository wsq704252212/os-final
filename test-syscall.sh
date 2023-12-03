#!/bin/bash

echo "Enable page cache..."
./run2 $1 4096 > /dev/null

echo "Start test..."
echo " "

echo "test read() syscall..."
./run2 $1 1 | grep B/s

echo " "

echo "test lseek() syscall..."
./run3 $1

