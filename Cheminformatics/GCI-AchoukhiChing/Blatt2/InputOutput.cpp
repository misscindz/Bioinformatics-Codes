////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Group: Ching, Achoukhi 											 ///
///  Blatt 2													 ///
///  														 ///
///  This file combines exercise 1 and 2                                                                         ///
///  for excercise 1:                                                                                            ///
///          use printInfoSingleMolecule(int option, MoleculePtr mol) for single molecule                        ///
///  for excercise 2:                                                                                            ///
///          use printInfoAllMolecules(MoleculeVector molvec,int option=1) for multiple molecules                ///
///          use printNumberOfMolecules(MoleculeVector molvec) to print total number of molecules                ///
///          use printAllElements(MoleculeVector molvec) to print all elements that occured in the molecule file ///
///  please uncomment functions you want to use in main()                                                        ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <cctype>

#include "Naomini/Molecule.hpp"
#include "Naomini/Atom.hpp"
#include "Naomini/MoleculeFactory.hpp"

using namespace Naomini;

namespace printOutput{

  //prints molecule or atom information, input is option (1 or 0) 
  //and a pointer to molecule
  void printInfoSingleMolecule(int option, MoleculePtr mol){
    //only molecule info when option is 0
    if (option == 0){
		std::cout << std::endl<< std::endl<<"Name of Molecule: " << mol->getName() << std::endl;
		std::cout << "- Number of Atoms: " << mol->getNofAtoms() << std::endl;
		std::cout << "- Atom Types: " << std::endl;
		for (const Atom* atom : mol->getAtoms()) {
            std::cout<< "\t" << atom <<std::endl;
		}
        std::cout << "Number of Bonds: "<< mol->getNofBonds() <<std::endl;
	}

    //only atom info when option is 1
    if (option == 1){
		std::cout << std::endl<< std::endl<<"Name of Molecule: " << mol->getName() << std::endl;
		std::cout<<"--------------------------------------------"<< std::endl;
		for (unsigned i = 0; i < mol->getNofAtoms(); i++ ) {
			std::cout <<std::endl<<std::endl<< "Name of Atom: " << mol->getAtoms().at(i)->getName()<<std::endl;
			std::cout << "- Atomic Charge: " << mol->getAtoms().at(i)->getAtomicCharge()<<std::endl;
			std::cout << "- Valence: " << mol->getAtoms().at(i)->getValence()<<std::endl;
			std::cout << "- Atomic Number: " << mol->getAtoms().at(i)->getAtomicNumber()<<std::endl;      
			std::cout << "- Atomic Weight: " << mol->getAtoms().at(i)->getAtomicWeight()<<std::endl;
			std::cout << "- Number of Bonds: " << mol->getAtoms().at(i)->getNofBonds()<<std::endl;	
			std::cout << "- ID: " << mol->getAtoms().at(i)->getID()<<std::endl;    
			std::cout << "- Neighbours: ";
			for (const Atom* atom1 : mol->getAtoms().at(i)->getNeighborAtoms()) {
	  			std::cout << atom1 << ", ";
			}
			std::cout << std::endl;
			}
		}
	}
  
  //prints info of all molecules in file, by default molecule info is printed
	void printInfoAllMolecules(MoleculeVector molvec,int option=1){
		for(unsigned int i=0; i != molvec.size(); i++){
            printInfoSingleMolecule(option, molvec.at(i));
    	}
  	}
  
  //prints number of all molecules in file
  	void printNumberOfMolecules(MoleculeVector molvec){
		std::cout << "Total number of molecules: "<< molvec.size() <<std::endl;
  	}
  

  	void printAllElements(MoleculeVector molvec){
    	std::vector<std::string> ElementVector(118);
    	int numberOfElements = 118;
    	std::string element;
    
    	//initialize ElementVector for all possible elements in periodic table
    	for (int k =0; k<numberOfElements; k++){
      		ElementVector.at(k) = "none";
    	}
    
    	//save all elements into Element
    	for (unsigned int i=0; i != molvec.size(); i++){
      		for (const Atom* atom : molvec.at(i)->getAtoms()) {
	  			//check if element is new using atomic number (count sort)
	  			if (ElementVector[atom->getAtomicNumber()] == "none"){
	    			element = (std::string)atom->getName();
	    			//remove digits
	    			element.erase(std::remove_if(element.begin(), element.end(), 
					&isdigit), element.end());
	    			ElementVector[atom->getAtomicNumber()] = element; 
	  			}	
      		}
    	}
    
    	//print all Elements without redundancy
    	std::cout <<std::endl<<"Elements occuring in this file" << std::endl;
    		for (unsigned int j = 0; j<ElementVector.size(); j++){ 
      			if (ElementVector.at(j) != "none"){
					std::cout << ElementVector.at(j) << "\t";
      			}
    		}
    	std::cout << std::endl;
  		}
	}


int main(int argc, char *argv[]) {
  
  int option = 0;
  
  //terminates program if not enough parameters
  if(argc != 3) {
    std::cerr << "Wrong number of parameters entered!" << std::endl;
    std::cerr << "Usage: " << std::string(argv[0])
	      << " <molecule file> <option>" <<  std::endl;    
    return EXIT_FAILURE;
  }
  //terminates program when no option entered
  if (!(sscanf(argv[2],"%d",&option)) || (option < 0) || (option > 1)) {
    std::cerr << "Option can only be 0 for printing molecule info or 1 for atom info" << std::endl;
    std::cerr << "Usage: " << std::string(argv[0])
	      << " <molecule file> <option>" <<  std::endl;    
    return EXIT_FAILURE;
  }
  
  // loads the first molecule of a given file  
  MoleculePtr mol = MoleculeFactory::getFirstMolecule(std::string(argv[1]));

  
  // loads all molecules of a given file
  MoleculeVector molvec= MoleculeFactory::getAllMolecules(std::string(argv[1]));
  

/////////////////////////////////////////////////////
///  PLEASE UNCOMMENT FUNCTIONS YOU WANT TO TEST  ///
/////////////////////////////////////////////////////

  printOutput::printInfoSingleMolecule(option, mol);
//  printOutput::printInfoAllMolecules(molvec,option);
//  printOutput::printNumberOfMolecules(molvec);
//  printOutput::printAllElements(molvec);
    
  return EXIT_SUCCESS;
}



