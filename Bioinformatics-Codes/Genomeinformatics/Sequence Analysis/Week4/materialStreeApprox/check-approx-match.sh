#!/bin/sh

checkerror() 
{
  if test $? -ne 0
  then
    echo "failure: ${cmd}"
    exit 1
  fi
}

if test $# -ne 4
then
  echo "Usage: $0 <referencefile> <indexname> <patternfile> <differences>"
  exit 1
fi

referencefile=$1
indexname=$2
patternfile=$3
differences=$4
cmd="./index.sh ${indexname} ${referencefile}"
${cmd}
checkerror
cmd="env -i ./stree-approx-match.x ${indexname} ${patternfile} ${differences}"
${cmd} > tmp.stree
checkerror
cmd="env -i ${GTDIR}/bin/gt tagerator -output abspos dbstartpos tagnum tagseq -nop -esa ${indexname} -q ${patternfile} -e ${differences}"
${cmd} > tmp.tagerator
checkerror
cmd="./compare_matches.rb tmp.stree tmp.tagerator"
${cmd}
checkerror
rm -f tmp.stree tmp.tagerator
