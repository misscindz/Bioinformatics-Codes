//Cindy Ching
//Blatt 5
//Aufgabe 2c,d

#pragma once

#include "Naomini/Forward.hpp"

namespace Naomini {

typedef struct Cluster Cluster;

double** getDistMat(MoleculeVector*, unsigned int, unsigned int);

void printDistMat(double** , unsigned int);
void printClusterPositions(Cluster&);

std::vector<Cluster>* completeLinkageClustering(MoleculeVector* ,
												double **, double );
void printSingleCluster(Cluster& cluster);
void printClusters(std::vector<Cluster>*, unsigned int );
void joinClusters(Cluster &, Cluster &,Cluster &);

} // end namespace Naomini
