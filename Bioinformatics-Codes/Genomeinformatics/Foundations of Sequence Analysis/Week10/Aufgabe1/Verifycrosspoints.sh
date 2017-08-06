#!/bin/sh

checkerror() {
  if test $? -ne 0
  then
    echo "failure: ${cmd}"
    exit 1
  fi
}

if test $# -ne 2
then
  echo "Usage: $0 <progname> <filename with one sequence per line>"
  exit 1
fi

aligner=$1
inputfile=$2

for s1 in `cat $inputfile`
do
  for s2 in `cat $inputfile`
  do
    if test $s1 != $s2
    then
      cmd="${aligner} ${mode} ${s1} ${s2}"
      ${cmd}
      checkerror
    fi
  done
done
