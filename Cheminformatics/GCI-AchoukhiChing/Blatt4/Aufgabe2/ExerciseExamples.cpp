//Aufgabe leider unvollstaendig

#include <iostream>

#include "Naomini/Atom.hpp"
#include "Naomini/Bond.hpp"
#include "Naomini/Molecule.hpp"
#include "Naomini/MoleculeFactory.hpp"
#include "Naomini/MoleculeDrawer.hpp"
#include "Naomini/Helpers.hpp"


namespace Naomini{

BondVector findAllDoubleBonds(MoleculePtr mol){
  BondVector doubleBonds;
  BondVector bondVector = mol->getBonds();
  for (unsigned int j = 0; j<bondVector.size(); j++){
    if (bondVector.at(j)->getType() == BondType::DOUBLE){
      doubleBonds.push_back(bondVector.at(j));
    }
  }
  return doubleBonds;
}

AtomVector findAllAmineNitrogens(MoleculePtr mol){
  AtomVector amineNitrogens;
  BondVector bondVector;
  AtomVector atomVector = mol->getAtoms();
  AtomVector atomPair;
  for (unsigned int i = 0; i<atomVector.size();i++){
    if (atomVector.at(i)->getAtomicNumber() == 7){
      bondVector = atomVector.at(i)->getBonds();
      for (BondPtr bond:bondVector){
	atomPair = bond->getAtoms();
	if ((atomPair.at(0).getAtomicNumber() == 1)||(atomPair.at(1).getAtomicNumber() == 1)){
	  amineNitrogens.push_back(atomVector.at(i));
	}
      }
    }
  }
  return amineNitrogens;
}

int main(int argc, char *argv[]) {
  // check number of arguments
  if (argc != 2){
    std::cout << "Usage: " << argv[0] << " <molecule-file> " << std::endl;
    return 1;
  }

  // get a molecule for each molecule entry in the provided file
  MoleculeVector mols = MoleculeFactory::getAllMolecules(std::string(argv[1]));

  // insert your code here!

  // delete all molecules
  for (MoleculePtr molecule : mols) {
    delete molecule;
  }
  return 0;
} // end main

} // end namespace Naomini






