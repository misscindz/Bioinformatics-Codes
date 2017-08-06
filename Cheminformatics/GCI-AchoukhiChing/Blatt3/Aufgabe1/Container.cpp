/* Was ist der Unterschied zwischen map und unordered_map? */
/* Maps sind implementiert als balancierte Suchbäume undorederd_map als hashing.*/
/* Die Laufzeit für das Hinzufügen, Suchen und Löschen liegt beim map in O(lg n) */
/* und unordered_map bei O(1)(im worstcase O(N)). */

/* Aufruf: Wenn diese Datei im Ordner Exercises/Container liegt: */
/* make Container */
/* ./Container MoleculeFiles/som_dud_ligand.smi */

#include <iostream>
#include <unordered_map>

#include "Naomini/Atom.hpp"
#include "Naomini/Bond.hpp"
#include "Naomini/Molecule.hpp"
#include "Naomini/MoleculeFactory.hpp"

using namespace std;
using namespace Naomini;

//insert method(s) here

namespace MoleculeAnalyser
{
  BondVector find_double_bond(MoleculePtr mol)
  {
    unsigned int i;
    BondVector doubleBondSet = {};
    BondVector bonds = mol->getBonds();
    for(i=0; i < bonds.size(); ++i)
    {
      if(bonds[i]->getType() == 2)
      {
	doubleBondSet.push_back(bonds.at(i));
      }
    }
    return doubleBondSet;
  }

  BondVector single_bond(MoleculePtr mol)
  {
    unsigned int i;
    BondVector singleBondSet = {};
    BondVector singleBonds = mol->getBonds();
    for(i=0; i<singleBonds.size(); ++i)
    {
      if(singleBonds[i]->getType() == 1)
      {
        singleBondSet.push_back(singleBonds.at(i));
      }
    }
    return singleBondSet;
  }

  AtomVector num0fAtom(MoleculePtr mol)
  {
    unsigned int i = 0;
    AtomVector atoms = mol->getAtoms();
    AtomVector results = {};
  
    while(i < atoms.size())
    {
      results.push_back(atoms.at(i));
      ++i;
    }
    return results;
  }

  AtomVector identify_n(MoleculePtr mol)
  {
    bool h_atom = false;
    bool single_bond = false;
    AtomVector::iterator m_iterator;
    AtomVector atom = mol->getAtoms();
    AtomVector neighborOfatom;
    AtomVector matches = {};
    for(m_iterator = atom.begin(); m_iterator != atom.end(); ++m_iterator)
    {
      unsigned int i,j;
      BondVector bonds = (*m_iterator)->getBonds();
      if((*m_iterator)->getAtomicNumber() == 7)
      {
        neighborOfatom = (*m_iterator)->getNeighborAtoms();
        for(i=0;i<bonds.size();++i)
        {
	  if(bonds[i]->getType() == SINGLE)
	  {
	    single_bond = true;
	  }
        }
        for(j=0; j < neighborOfatom.size(); ++j)
        {
	  if(neighborOfatom[j]->getAtomicNumber() == 1)
	  {
	    h_atom = true;
	  }	
        }
        if (single_bond == true && h_atom == true)
        {
          matches.push_back(*m_iterator);
        }
      }
    }
    return matches;
  }
}

int main(int argc, char *argv[]) 
{
  unsigned int i;

  if(argc != 2) 
  {
    std::cerr << "Usage: <Molecule file>" << std::endl;
    return 1;
  }

  MoleculeVector inMols = MoleculeFactory::getAllMolecules(std::string(argv[1]));

  for (MoleculePtr molecule : inMols) 
  {
    std::cout << "Molecule "
              << molecule->getName()
             /* << " has "
              << "%insert number of atoms%"
              << " atoms and "
              */
              << std::endl;
	      
    AtomVector nof_atoms = MoleculeAnalyser::num0fAtom(molecule);
    cout<<"Die Anzahl der Atome im Molekül ist: "<< nof_atoms.size()<<endl;
    
    BondVector single_bonds = MoleculeAnalyser::single_bond(molecule);
    cout<<"Die Anzahl der Einzelbindungen im Molekül ist: "<< single_bonds.size()<<endl;
    
    BondVector double_bonds = MoleculeAnalyser::find_double_bond(molecule);
      cout<<"Die Anzahl an Doppelbindungen im Molekül ist: "<< double_bonds.size()<<endl;
    
      AtomVector n_atom = MoleculeAnalyser::identify_n(molecule);
      cout<<"Die Anzahl an N-Atome, die nur Einzelbindungen besitzen und ein H-Atom als Nachbar:\n";
      for(i=0; i<n_atom.size(); i++)
      {
	cout<<n_atom[i]<<endl;
      }
	    
  }
  
  // delete all molecules
  for (MoleculePtr molecule : inMols) 
  {
    delete molecule;
  }

  return 0;
}
