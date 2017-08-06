#!/bin/bash

if [[ $# -ne 1 ]]
then
  echo "USAGE: $0 <path>"
  exit 1
fi

sequences=(
SISMLYLNEQNKVVLKNYQDMTVVGCGCR
MAGPVLYQDRAMKQITFAPRNHLLTNTNTWTPDSQW
LVFDVRPSGASFTGETIERVNIHTGEVEVIYRAS
EYSGSHWCVLVSKTTPTPQPGS
LMAMNEHCGKPLNDTRLLALMGELEGRISGSIHY
)
thresh=(
5
30
)
modes=(
s
l
)

for m in ${modes[@]}
do
  for seq in ${sequences[@]}
  do
    echo "seq: $seq"
    for th in ${thresh[@]}
    do
      echo "threshold: $th"
      $1 $m ./pssm_aus_skript.pssm $seq $th
    done
  done
done
