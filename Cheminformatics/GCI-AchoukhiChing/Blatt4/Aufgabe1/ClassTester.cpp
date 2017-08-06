#include <iostream>
#include "Naomini/Molecule.hpp"
#include "Naomini/MoleculeFactory.hpp"
#include "MolFilter.hpp"

using namespace Naomini;

int main(int argc, char *argv[]) {


  if(argc != 2) {
    std::cerr << "Usage: <Molecule file>" << std::endl;
    return 1;
  }

  MoleculeVector mols = MoleculeFactory::getAllMolecules(std::string(argv[1]));
  MolFilter filter(mols);

  std::cout << "---Atom count---" << std::endl;
  if (filter.getMoleculesByAtomCount(Equal, 28).size() == 24) {
    std::cout << "  SUCCESS: Equal filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Equal filter fails!" << std::endl;
  }
  if (filter.getMoleculesByAtomCount(LessThan, 20).size() == 6) {
    std::cout << "  SUCCESS: Less than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Less than filter fails!" << std::endl;
  }
  if (filter.getMoleculesByAtomCount(MoreThan, 40).size() == 628) {
    std::cout << "  SUCCESS: More than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: More than filter fails!" << std::endl;
  }

  std::cout << "---Heavy atom count---" << std::endl;
  if (filter.getMoleculesByHeavyAtomCount(Equal, 17).size() == 27) {
    std::cout << "  SUCCESS: Equal filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Equal filter fails!" << std::endl;
  }
  if (filter.getMoleculesByHeavyAtomCount(LessThan, 15).size() == 15) {
    std::cout << "  SUCCESS: Less than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Less than filter fails!" << std::endl;
  }
  if (filter.getMoleculesByHeavyAtomCount(MoreThan, 35).size() == 18) {
    std::cout << "  SUCCESS: More than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: More than filter fails!" << std::endl;
  }

  std::cout << "---Bond count---" << std::endl;
  if (filter.getMoleculesByBondCount(Equal, 32).size() == 41) {
    std::cout << "  SUCCESS: Equal filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Equal filter fails!" << std::endl;
  }
  if (filter.getMoleculesByBondCount(LessThan, 15).size() == 0) {
    std::cout << "  SUCCESS: Less than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Less than filter fails!" << std::endl;
  }
  if (filter.getMoleculesByBondCount(MoreThan, 50).size() == 169) {
    std::cout << "  SUCCESS: More than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: More than filter fails!" << std::endl;
  }

  std::cout << "---Double bond count---" << std::endl;
  if (filter.getMoleculesByDoubleBondCount(Equal, 4).size() == 26) {
    std::cout << "  SUCCESS: Equal filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Equal filter fails!" << std::endl;
  }
  if (filter.getMoleculesByDoubleBondCount(LessThan, 2).size() == 997) {
    std::cout << "  SUCCESS: Less than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Less than filter fails!" << std::endl;
  }
  if (filter.getMoleculesByDoubleBondCount(MoreThan, 5).size() == 0) {
    std::cout << "  SUCCESS: More than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: More than filter fails!" << std::endl;
  }

  std::cout << "---Ring count---" << std::endl;
  if (filter.getMoleculesByRingCount(Equal, 4).size() == 499) {
    std::cout << "  SUCCESS: Equal filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Equal filter fails!" << std::endl;
  }
  if (filter.getMoleculesByRingCount(LessThan, 3).size() == 105) {
    std::cout << "  SUCCESS: Less than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: Less than filter fails!" << std::endl;
  }
  if (filter.getMoleculesByRingCount(MoreThan, 5).size() == 2) {
    std::cout << "  SUCCESS: More than filter works!" << std::endl;
  } else {
    std::cout << "  ERROR: More than filter fails!" << std::endl;
  }

  return 0;
}

