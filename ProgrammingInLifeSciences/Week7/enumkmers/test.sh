#!/bin/sh

for i in 2 4 7
do
  for A in "ABC" "acB" "1_XG"
  do
    ./enumkmers.x ${A} ${i}
  done
done
