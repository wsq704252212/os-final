#!/bin/bash

rm raw

echo "Enable page cache"
./run2 $1 4096 > /dev/null

echo "Start test..."

for (( i=4096; i < 4096 * 101; i=i+4096 ))
do
  ./run2 $1 $i >> raw
done

cat raw | grep speed | awk '{print $3}' > raw-speed
