#!/bin/sh

if test $# -eq 0
then
  echo "$0 requires at least one argument, namely 1 and/or 2"
  exit 1
fi

GB=ftp://ftp.ncbi.nlm.nih.gov/genomes/archive/old_genbank/Bacteria

# Skript to download two Ecoli genomes if not available yet.

for genome in $*
do
  if test ${genome} -eq 1
  then
    if test ! -f Ecoli_K12.fna
    then
      curl -o Ecoli_K12.fna ${GB}/Escherichia_coli_K_12_substr__MG1655_uid225/U00096.fna
    fi
  fi
  if test ${genome} -eq 2
  then
    if test ! -f Ecoli_O127_H6.fna
    then
      curl -o Ecoli_O127_H6.fna ${GB}/Escherichia_coli_0127_H6_E2348_69_uid32571/FM180568.fna
    fi
  fi
done
