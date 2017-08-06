// Blatt2 Aufgabe 2: Cindy Ching, Asmaa Achoukhi
// 01.05.17
#include "CompatibilityGraph.hpp"

#include <stdexcept>

#include "Naomini/Forward.hpp"
#include "Naomini/Molecule.hpp"
#include "Naomini/Bond.hpp"

namespace Naomini {

/* Helper Function ************************************************************/
namespace {

bool moleculeHasBond(MoleculePtr mol, AtomPtr atom1, AtomPtr atom2)
{
  for (BondPtr bond : mol->getBonds()) {
    AtomPair atoms = bond->getAtoms();
    if ((atoms.first == atom1 && atoms.second == atom2) ||
        (atoms.first == atom2 && atoms.second == atom1)) {
      return true;
    }
  }
  return false;
}

} // end anonymouse namespace
/* Helper Function END ********************************************************/

/* Compatibility Class Functions **********************************************/

CompatibilityGraph::CompatibilityGraph(MoleculePtr mol1, MoleculePtr mol2)
{
  unsigned atomic_num_of_atom_1;
  unsigned atomic_num_of_atom_2;

/* Hier bitte den Code zur initializierung des Graphen einfuegen. */
  // Ueber jedes Atom im Molekül mol1 wird iteriert.
  for( AtomPtr atom1 : mol1->getAtoms())
  {
    atomic_num_of_atom_1 = atom1->getAtomicNumber();
    // Wenn es sich um ein H-Atom handelt so fahre fort mit dem naechsten Atom. 
    if(atomic_num_of_atom_1 == ATOMIC_NUM_1) continue;
    // Ueber jedes Atom im Molekül mol2 wird iteriert.
    for (AtomPtr atom2 : mol2->getAtoms())
    {
        atomic_num_of_atom_2 = atom2->getAtomicNumber();
      // Wenn es sich um ein H-Atom handelt so fahre fort mit dem naechsten Atom. 
      if(atomic_num_of_atom_2 == ATOMIC_NUM_1) continue;
      // Wenn atom1 und atom2 vom selben Elementtyp sind, so füge die beiden Atome 
      // in den Vektor "nodes" ein.
      if(atomic_num_of_atom_1 == atomic_num_of_atom_2)
      {
	    this->nodes.push_back(Node((atomic_num_of_atom_1), atom1, atom2));
      }
    }
    // für jeden Knoten "node1" in nodes und jeden Knoten "node2" in nodes 
    // wird iteriert.
    for(Node node1 : this->nodes)
    {
      for (Node node2 : this->nodes)
      {
        // Wenn es sich um dieselben Knoten handelt so fahre fort.
	    if(node1 == node2) continue;

	    // MoleculeHasBond wird aufgerufen um zu testen, ob es eine Bindung
	    // zwischen Atom1 und atom2 existiert. Und dies in beiden Molekuelen.
	    // ==> C-Kanten: Die Knoten sind jeweils mit einer Kante verbunden.
	    if(moleculeHasBond(mol1, node1.atom1, node2.atom1)&&
	       moleculeHasBond(mol2, node1.atom2, node2.atom2))
	    {
	      // Eine Kante wird zwischen den Knoten gebildet.
	      this->edges.push_back(Edge(node1,node2));
	     }
	     // Mit der selben Funktion testen, ob atom 1 und atom2 keine Bindungen
	    // in beiden Molekuelen aufweisen.
	    // ==> D-Kanten: Die Knoten sind jeweils nicht mit einer Kante verbunden.
	    if(!(moleculeHasBond(mol1, node1.atom1, node2.atom1))&&
	       !(moleculeHasBond(mol2,node1.atom2, node2.atom2)))
	    {
	      this->edges.push_back(Edge(node1,node2));
	    }
      }
    }
  }
}

CompatibilityGraph::~CompatibilityGraph()
{}

size_t CompatibilityGraph::getNofNodes()
{
/* Hier bitte den Code zur Berechnung der Anzahl der Knoten einfuegen. */
  
  // Durch nodes.size() wendet man die Funktion size() auf den Vektor nodes 
  // an. So erhalet man die Anzahl an Knoten
  
  return this->nodes.size();
}

std::vector<Node> CompatibilityGraph::getNodes() const
{
/* Hier bitte den Code zur Abfrage der Knoten eingeben. */
  
  // liefert den Vektor "nodes" zurueck.
  
  return this->nodes;
}

Node CompatibilityGraph::getNode(unsigned i) const
{
/* Hier bitte den Code zur Abfrage eines Knotens eingeben. */
  
  // liefert den Knoten an der Stelle i zurueck.
  
  return this->nodes[i];
}

bool CompatibilityGraph::hasEdge(const Node &a, const Node &b) const
{
/* Hier bitte den Code zur Abfrage der Existenz einer Kante einfuegen. */

  // Iteriere über alle Kanten aus dem Vektor "edges".
  for (Edge edge : this->edges)
  {
    // Vergleich der beiden Knoten node1 und node2 mit den übergebenen Knoten a und b.
    if((edge.node1 == a && edge.node2 == b) || (edge.node1 == b && edge.node2 == a))
    {
      return true;
    }
    
  }
  return false;
}
}

