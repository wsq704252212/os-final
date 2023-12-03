#!/bin/bash

rm raw-noncache
rm raw-cache

clearcmd='sync && sudo purge'
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  clearcmd='echo 3 > /proc/sys/vm/drop_caches'
fi

echo "clear page cache"
eval "$clearcmd"

echo "Start test..."

for (( i=4096; i < 4096 * 101; i=i+4096 ))
do
  eval "$clearcmd"
  ./run2 $1 $i >> raw-noncache
  ./run2 $1 $i >> raw-cache
done

cat raw-noncache | grep speed | awk '{print $3}' > raw-noncache-speed
cat raw-cache | grep speed | awk '{print $3}' > raw-cache-speed
