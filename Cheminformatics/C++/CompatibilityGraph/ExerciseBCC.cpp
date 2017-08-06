//CIW:Blatt3
//Gruppe: Ching, Achoukhi


#include <iostream>

#include "Naomini/Atom.hpp"
#include "Naomini/Bond.hpp"
#include "Naomini/Molecule.hpp"
#include "Naomini/MoleculeFactory.hpp"
#include "Naomini/MoleculeDrawer.hpp"
#include "Naomini/Helpers.hpp"

#include "RingsAndBiconnectedComponents.hpp"
namespace Naomini{

int main(int argc, char *argv[]) {
  // check the number of arguments
  if (argc != 2){
    std::cout << "Usage: " << argv[0] << " <mol2-file>" << std::endl;
    return 1;
  }
  // get a molecule for each molecule entry in the provided file
  MoleculeVector mols = MoleculeFactory::getAllMolecules(std::string(argv[1]));
  try{
    // calculate and draw all rings of all molecules
    for (MoleculePtr mol : mols) {
    	std::cout << "mol name: "<<mol->getName()<<std::endl;
		AtomVector substructure;

	  // markiert Ringe: Bitte auskommentieren, wenn nicht benoetigt.
      Naomini::MoleculeDrawer drawer1(mol);
      RingVector rings = moleculeGetRings(mol);
      drawer1.markSubstructures(rings);
      /////////////////////////////////////////////////////////////////

      //markiert Ringe mit endstaendigem Schweratom: Bitte entkommentieren, wenn benoetigt.
      /////////////////////////////////////////////////////////////////////////////////////
//      Naomini::MoleculeDrawer drawer2(mol);
//      RingVector extendedRings = moleculeGetExtendedRings(mol);
//      drawer2.markSubstructures(extendedRings);
      /////////////////////////////////////////////////////////////////

      //soll BCCs markieren, funktioniert nicht. Bitte entkommentieren, wenn benoetigt.
      ////////////////////////////////////////////////////////////////////////////////
//      RingVector extendedRings = moleculeGetExtendedRings(mol);
//      Naomini::MoleculeDrawer drawer3(mol);
//      BCCVector bccs = moleculeGetBiconnectedComponents(mol);
//      for(AtomPtr atom : extendedRings.at(0)){
//    	  substructure.push_back(atom);
//      }
//      drawer3.markSubstructures(bccs);
//      drawer3.markSubstructure(substructure, MoleculeDrawer::BLUE);
    }
  }
  catch (const char *err){
    std::cerr << err << std::endl;
  }

  // delete all molecules
  for (MoleculePtr molecule : mols) {
    delete molecule;
  }
  return 0;
}

} // end namespace Naomini
