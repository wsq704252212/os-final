#!/bin/bash

rm fast-time

clearcmd='sync && sudo purge'
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  echo "linux-gnu"
  clearcmd='echo 3 > /proc/sys/vm/drop_caches'
fi

echo "clear page cache..."
eval "$clearcmd"

echo "Start test..."
 
for (( i=40960*7; i < 40960 * 19; i=i+4096 ))
do
  eval "$clearcmd"
  { time -p ./fast $1 $2 $i; } 2>> fast-time
done

cat fast-time | grep real | awk '{print $2}' > fast-realtime
