#pragma once

#include "Naomini/Forward.hpp"

namespace Naomini{

/**
 * Operators for the evaluation of filters.
 */
enum Operator {
  Equal = 0,
  MoreThan,
  LessThan
};


class MolFilter {

public:
    //constructor
    MolFilter(MoleculeVector molvec);
    
    //methods

    MoleculeVector getMoleculesByAtomCount(Operator op, int value);
    MoleculeVector getMoleculesByHeavyAtomCount(Operator op, int value);
    MoleculeVector getMoleculesByBondCount(Operator op, int value);
    MoleculeVector getMoleculesByDoubleBondCount(Operator op, int value);
    MoleculeVector getMoleculesByRingCount(Operator op, int value);

private:
    MoleculeVector moleculeVector;
    unsigned int numOfMolecules;
    bool push_this(int op,int tiredAF,int whoCares);

};

}
