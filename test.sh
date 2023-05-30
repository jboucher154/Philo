#!/bin/bash

for i in $(seq 1 100)
do
echo $i
./philo 199 600 200 200 > out_$i
tail -n 1 out_$i
done