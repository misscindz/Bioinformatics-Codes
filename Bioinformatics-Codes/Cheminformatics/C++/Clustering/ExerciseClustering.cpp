//Cindy Ching
//Blatt 5
//Aufgabe 2c,d

#include <iostream>
#include <stdlib.h>

#include "Naomini/Forward.hpp"
#include "Naomini/Molecule.hpp"
#include "Naomini/MoleculeFactory.hpp"

#include "CompleteLinkageClustering.hpp"

using namespace Naomini;



int main(int argc, char *argv[]) {
  // check the number of arguments
  if (argc != 3){
    std::cout << "Usage: " << argv[0]
              << " <molecule-file> <similarity treshold >" << std::endl;
    return 1;
  }

  // read the similarity threshold from argument 2
  double threshold = atof(argv[2]);
  if (threshold < 0 || threshold > 1){
    std::cout << "Use a sensible number of iterations." << std::endl
        << "Usage: " << argv[0]
                     << " <molecule-file> <similarity treshold >" << std::endl;
    return 1;
  }

  // get a molecule for each molecule entry in the provided file
  MoleculeVector mols = MoleculeFactory::getAllMolecules(std::string(argv[1]));
  unsigned len = mols.size();
  double **distMat = getDistMat(&mols, (unsigned int)threshold, len);

  //print distance Matrix
  printDistMat(distMat,len);

  // cluster molecules and create output
  completeLinkageClustering(&mols, distMat,threshold);

  // delete all molecules
  for (MoleculePtr molecule : mols) {
    delete molecule;
  }

  return 0;
}


