//CIW:Blatt3
//Gruppe: Ching, Achoukhi

#include "RingsAndBiconnectedComponents.hpp"

#include "Naomini/Atom.hpp"
#include "Naomini/Bond.hpp"
#include "Naomini/Forward.hpp"
#include "Naomini/Helpers.hpp"
#include "Naomini/Molecule.hpp"
#include "Naomini/MoleculeDrawer.hpp"
#include "Naomini/MoleculeFactory.hpp"

#include <stdbool.h>
#include <iostream>
#include <algorithm> //zum removen

using namespace Naomini;

void dfs(AtomPtr x, BondPtr bond, std::vector<bool> *visited_atoms){
	AtomPtr w;
	(*visited_atoms)[x->getID()]=true;
	BondVector bonds_with_neighbors = x->getBonds();
	for (BondPtr b : bonds_with_neighbors){
		if (bond != b){
			w = b->getOpposite(x);
			if((*visited_atoms)[w->getID()]==false){
				dfs(w,b,visited_atoms);
			}
		}
	}
}

/*----------------------------------------------------------------------------*/

void addHeavyNeighborAtom(AtomPtr atom, Ring *ringatoms){
	AtomVector neighbors = atom->getNeighborAtoms();
	unsigned int num_of_neighbors;
	for (AtomPtr neighbor_atom : neighbors){
		 num_of_neighbors = (neighbor_atom->getNeighborAtoms()).size();
		if (num_of_neighbors <=2  && !atomIsHydrogen(neighbor_atom)){
			ringatoms->insert(neighbor_atom);
		}
	}
}

/*----------------------------------------------------------------------------*/

bool isCyclic(BondPtr bond, AtomVector atoms, BondVector bonds){
	BondVector new_bonds = bonds; //malloc?
	std::vector <bool> visited_atoms (atoms.size());
	new_bonds.erase(std::remove(new_bonds.begin(),new_bonds.end(),bond),new_bonds.end());
	AtomPair xy_pair = bond->getAtoms();
	AtomPtr x = xy_pair.first;
	AtomPtr y = xy_pair.second;

	for(AtomPtr a : atoms){
		visited_atoms.at(a->getID()) = false;
	}

	dfs(x,bond,&visited_atoms);
	if (visited_atoms[y->getID()]==true){
		return true;
	}
	return false;
}

/*----------------------------------------------------------------------------*/

void hasRing(AtomPtr x, BondPtr bond, AtomVector atoms, BondVector bonds, bool *found_ring){
	AtomPtr w;
	BondVector bonds_with_neighbors = x->getBonds();
	for (BondPtr b : bonds_with_neighbors){
		if (bond != b){
			if (isCyclic(b,atoms,bonds)){
				*found_ring = true;
				return; //wenn ring vorhanden, dann abbruch

			}
			w = b->getOpposite(x);
			hasRing(w,b,atoms,bonds,found_ring);
		}
	}
}

/*----------------------------------------------------------------------------*/

RingVector Naomini::moleculeGetRings(MoleculePtr mol){
	BondVector all_bonds = mol->getBonds();
	Ring ringatoms;
	RingVector rings;
	AtomPair xy_pair;
	AtomVector atoms = mol->getAtoms();
	AtomPtr x,y;

	for(BondPtr e : all_bonds){
		if (isCyclic(e,atoms,all_bonds)){
			xy_pair = e->getAtoms();
			x=xy_pair.first;
			y=xy_pair.second;
			ringatoms.insert(x);
			ringatoms.insert(y);
		}
	}
	rings.push_back(ringatoms);
	return rings;

	// Insert your code here but do not use this function for your solution!
    //RingVector rings = getRingsOfMolecule(mol);
}

/*----------------------------------------------------------------------------*/

RingVector Naomini::moleculeGetExtendedRings(MoleculePtr mol){
	RingVector rings = moleculeGetRings(mol);
	unsigned int ringidx = 0;
	for(Ring ring : rings){
		for(AtomPtr atom : ring){
			addHeavyNeighborAtom(atom, &ring);
			rings.at(ringidx)=ring;
		}
		ringidx++;
	}
	// Insert your code here
  return rings;
}

/*----------------------------------------------------------------------------*/

BCCVector Naomini::moleculeGetBiconnectedComponents(MoleculePtr mol){
  BCCVector allBCCs;
  std::set <BondPtr> non_cyclic_bonds;
  Ring linker;
  AtomPair xy_pair;
  AtomVector atoms = mol->getAtoms();
  AtomPtr x,y;
  BondVector bonds = mol->getBonds();
  bool left_has_ring = false;
  bool right_has_ring = false;

  for(BondPtr bond : bonds){
	  if(!isCyclic(bond,atoms,bonds)){
		  non_cyclic_bonds.insert(bond);
	  }
  }

  for(BondPtr bond : non_cyclic_bonds){
	  xy_pair = bond->getAtoms();
	  x=xy_pair.first;
	  y=xy_pair.second;
	  hasRing(x,bond,atoms,bonds,&left_has_ring);
	  hasRing(y,bond,atoms,bonds,&right_has_ring);

	  if((left_has_ring)&&(right_has_ring)){
		  if(!atomIsHydrogen(x)&& x->getNeighborAtoms().size()>1){
			linker.insert(x);
		  }
		  if(!atomIsHydrogen(y)&&y ->getNeighborAtoms().size()>1){
		  	linker.insert(y);
		  }

	  }
  }
  allBCCs.push_back(linker);

//  throw "Insert your code in RingsAndBiconnectedComponents.cpp";
  return allBCCs;
}

