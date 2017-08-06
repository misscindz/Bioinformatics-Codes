#!/bin/sh -e

if [ $# -ne 1 ]
then
  echo "usage: $0 path"
  echo "where path is the path to your binary"
  echo "compare the output given by this script to testdata.out to verify "
  echo "that your program works correctly."
  exit 1
fi

while read line ; do $1 $line ; done < ./testdata.txt
