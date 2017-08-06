#!/bin/sh

GTTAR=genometools-unstable.tar.gz
if test ! -f ${GTTAR}
then
  curl -o ${GTTAR} http://genometools.org/pub/${GTTAR}
fi
