//Cindy Ching
//Blatt 5
//Aufgabe 2c,d

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "Naomini/Forward.hpp"
#include "Naomini/Molecule.hpp"
#include "Naomini/MoleculeFactory.hpp"

#include "CompleteLinkageClustering.hpp"

using namespace Naomini;

namespace Naomini{

typedef struct Cluster{
	std::vector<unsigned int> mol_pos;
	int ID;
}Cluster;

double** getDistMat(MoleculeVector* mols, unsigned iterations, unsigned int numOfMolecules){
	//deklariert 2d Matrix
	double** distMat = new double*[numOfMolecules];

	//initialisiert 2d-matrix auf 0
	for (unsigned int i = 0; i < numOfMolecules; i++){
		distMat[i] = new double[numOfMolecules];
		for (unsigned int m = 0; m<numOfMolecules; m++){
			distMat[i][m]=0;
		}
	}

	//berechnet fuer jedes Molekuel den ECPF und fuer jedes ECFP den Tanimotokoeffizienten
	//dieser wird dann in der Matrix gespeichert
	for (unsigned int k = 0; k < numOfMolecules; k++) {
		for (unsigned int l = 0; l < numOfMolecules; l++){
			ECFP ecfp_k = getECFPOfMolecule(mols->at(k), 2);
			ECFP ecfp_l = getECFPOfMolecule(mols->at(l), 2);
			distMat[k][l]= getTanimotoCoefficientOfECFPs(ecfp_k, ecfp_l);
		}
	}
	return distMat;
}

//printed die Distanzmatrix aus
void printDistMat(double **Mat, unsigned len){
	std::cout << "Distance Matrix: " << std::endl;
	for (unsigned i =0; i<len ; i++){
		for (unsigned j=0; j<len; j++){
			std::cout << std::setprecision(2)<<Mat[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

//fuegt zwei Cluster zusammen
void joinClusters(Cluster &tmp_cluster, Cluster &a_cluster,Cluster &b_cluster){
			if(!(tmp_cluster.mol_pos.empty())){
					tmp_cluster.mol_pos.clear();
			}
			tmp_cluster.mol_pos.insert(tmp_cluster.mol_pos.end(),
											a_cluster.mol_pos.begin(),
											a_cluster.mol_pos.end());
			tmp_cluster.mol_pos.insert(tmp_cluster.mol_pos.end(),
											b_cluster.mol_pos.begin(),
											b_cluster.mol_pos.end());
			tmp_cluster.ID = a_cluster.ID;
}

//entfernt Cluster aus Clustervektor
void removeCluster(std::vector<Cluster> &clusters, int ID){
	clusters.erase(
			std::remove_if(clusters.begin(),clusters.end(), [&] (Cluster const & cluster){
					return cluster.ID == ID;
					}),
					clusters.end());
}

//printed einzelnes CLuster
void printSingleCluster(Cluster& cluster){
	std::cout<<"Cluster "<<cluster.ID<<"= {";
	for(unsigned int position : cluster.mol_pos){
		if (position != cluster.mol_pos.back()){
			std::cout<<position<<",";
		}
		else{
			std::cout<<position;
		}
	}
	std::cout<<"}"<<std::endl;
}

//printed im Clustervektor alle Cluster und die jeweiligen Positionen aus
void printClusters(std::vector<Cluster>* clusters, unsigned int len){
	for(unsigned int i = 0; i<len; i++){
		printSingleCluster(clusters->at(i));
	}
}

std::vector<Cluster>* completeLinkageClustering(MoleculeVector* mols,
												double **Mat, double mindist){
	unsigned int len = mols->size(),
						prevlen,
						i=0;

	double d=1000; //initial value high similarity
	std::vector<Cluster> clusters(len);
	std::vector<Cluster>* returncluster = new std::vector<Cluster> [len];//nur zum returnen
	Cluster new_cluster;
	Cluster tmp_cluster_a;
	Cluster tmp_cluster_b;

	//create singletons
	for(unsigned int i=0; i<len; i++){
			clusters[i].mol_pos.push_back(i);
			clusters[i].ID = i;
	}

	std::cout<<std::endl<<"START CLUSTERS";
	std::cout<<std::endl<<"--------------"<<std::endl;
	printClusters(&clusters,len);

	while(clusters.size() > 1){
		for(Cluster a : clusters){
			for(Cluster b : clusters){
				if (a.ID != b.ID){
					for(unsigned int j:a.mol_pos){
						for(unsigned int k:b.mol_pos){
							if(Mat[j][k]<d && Mat[j][k]>mindist){
								d=Mat[j][k]; //minimale Aehnlichkeit = maximale distanz
								tmp_cluster_a = a;
								tmp_cluster_b = b;

							}

						}
					}
				}
			}
		}
		prevlen = clusters.size();

		joinClusters(new_cluster,tmp_cluster_a,tmp_cluster_b);
		removeCluster(clusters,tmp_cluster_a.ID);
		removeCluster(clusters,tmp_cluster_b.ID);
		clusters.push_back(new_cluster);

		len = clusters.size();
		if (prevlen == len){

			returncluster = &clusters;
			return returncluster;
		}
		std::cout<<"\nCLUSTERING LEVEL "<<++i<<std::endl;
		printClusters(&clusters,len);
		d=1000;
	}
	return returncluster;
}


}
