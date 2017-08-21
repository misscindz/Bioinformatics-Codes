////////////////////////////////
///  			     ///
///  Group: Ching, Achoukhi  ///
///  Blatt 4: Aufgabe 1	     ///			
///  			     ///	
////////////////////////////////


#include "MolFilter.hpp"

#include "Naomini/Helpers.hpp"
#include "Naomini/Molecule.hpp"
#include "Naomini/Atom.hpp"
#include "Naomini/Bond.hpp"
#include <stdbool.h>

using namespace Naomini;

//constructor
MolFilter::MolFilter(MoleculeVector molvec){
	moleculeVector = molvec;
    numOfMolecules = moleculeVector.size();      
}
    
//methods
bool MolFilter::push_this(int op,int tiredAF,int whoCares){
    bool push = false;
        if (op == Equal){
            if (tiredAF == whoCares){
                push = true;
            }
        }
        if (op == MoreThan){
            if (tiredAF > whoCares){
                push = true;
            }
        }
        if (op == LessThan){
            if (tiredAF < whoCares){
                push = true;
            }
    }
    return push;
}
    
MoleculeVector MolFilter::getMoleculesByAtomCount(Operator op, int value){
      
	MoleculeVector filteredMolecules;
	int numOfAtoms = 0;
      
	for (unsigned int i= 0; i<numOfMolecules; i++){
		numOfAtoms = moleculeVector.at(i)->getNofAtoms();
 
        if (push_this(op,numOfAtoms,value)){
            filteredMolecules.push_back(moleculeVector.at(i));
        }
    }
    return filteredMolecules;     
}
    
MoleculeVector MolFilter::getMoleculesByHeavyAtomCount(Operator op, int value){
      
	MoleculeVector filteredMolecules;
	int heavyAtoms;
      
	for (unsigned int i= 0; i<numOfMolecules; i++){
		heavyAtoms = 0;
		unsigned int numOfAtoms = moleculeVector.at(i)->getNofAtoms();
	
		for (unsigned int j = 0; j<numOfAtoms; j++){	    
			if (!(atomIsHydrogen(moleculeVector.at(i)->getAtoms().at(j)))){
				heavyAtoms++;
	  		}			
		}
        if (push_this(op,heavyAtoms,value)){
            filteredMolecules.push_back(moleculeVector.at(i));
        }
	}
	return filteredMolecules;
} 
    
MoleculeVector MolFilter::getMoleculesByBondCount(Operator op, int value){
      
	MoleculeVector filteredMolecules;
	int numOfBonds = 0;
      
	for (unsigned int i= 0; i<numOfMolecules; i++){
		numOfBonds = moleculeVector.at(i)->getNofBonds();
	
        if (push_this(op,numOfBonds,value)){
            filteredMolecules.push_back(moleculeVector.at(i));
        }
	}  

    return filteredMolecules;
}
    
MoleculeVector MolFilter::getMoleculesByDoubleBondCount(Operator op, int value){
      
	MoleculeVector filteredMolecules;
	int doubleBonds;
      
	for (unsigned int i= 0; i<numOfMolecules; i++){
		doubleBonds = 0;
		BondVector bondVector = moleculeVector.at(i)->getBonds();
		for (unsigned int j = 0; j<bondVector.size(); j++){
	  		if (bondVector.at(j)->getType() == BondType::DOUBLE){
	    			doubleBonds++;
	  		}
		}
        if (push_this(op,doubleBonds,value)){
            filteredMolecules.push_back(moleculeVector.at(i));
        }
	}     
	return filteredMolecules;
}   
    
MoleculeVector MolFilter::getMoleculesByRingCount(Operator op, int value){
      
	MoleculeVector filteredMolecules;
	int rings;
	RingVector getRingsOfMolecule(MoleculePtr mol);
	for (unsigned int i= 0; i<numOfMolecules; i++){
		RingVector ringVector = getRingsOfMolecule(moleculeVector.at(i));
		rings = ringVector.size();
        if (push_this(op,rings,value)){
            filteredMolecules.push_back(moleculeVector.at(i));
        }     
    	return filteredMolecules;
}
    
