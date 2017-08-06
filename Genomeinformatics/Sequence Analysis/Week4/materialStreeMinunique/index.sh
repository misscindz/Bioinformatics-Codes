#!/bin/sh

checkerror() 
{
  if test $? -ne 0
  then
    echo "failure: ${cmd}"
    exit 1
  fi
}

if test $# -lt 2
then
  echo "Usage: $0 <indexname> <file1> [file2]"
  exit 1
fi

indexname=$1
shift
files=$*

options="-dc 256 -v -dna -sds no -des no -suf -lcp -tis -md5 no"
gtbin=${GTDIR}/bin/gt
if test ! -f ${indexname}.prj
then
  cmd="${gtbin} suffixerator ${options} -indexname ${indexname} -db ${files}"
  ${cmd}
  checkerror
fi
