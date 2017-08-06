#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "Naomini/Forward.hpp"
#include "Naomini/Atom.hpp"

#define ATOMIC_NUM_1	1


namespace Naomini {

class Node {

public:
  Node(unsigned element, AtomPtr atom1, AtomPtr atom2)
    :element(element), atom1(atom1), atom2(atom2)
  {}

  unsigned element;
  AtomPtr atom1;
  AtomPtr atom2;

  bool operator== (const Node& n) const
  {
    return (element == n.element)
        && (atom1->getID() == n.atom1->getID())
        && (atom2->getID() == n.atom2->getID());
  }

  bool operator< (const Node& n) const
  {
    if (element != n.element) {
      return element < n.element;
    }
    else if (atom1->getID() != n.atom1->getID())
    {
      return atom1->getID() < n.atom1->getID();
    }
    else {
      return atom2->getID() < n.atom2->getID();
    }
  }

  friend std::ostream& operator<< (std::ostream& os, const Node& n)
  {
    os << n.element << "(" << n.atom1->getID() << "," << n.atom2->getID() << ")";
    return os;
  }
};

/** Graph Klasse zur Nutzung in BronKerbosch.hpp */

class Edge{
  //Initialisieren der Klassenmember aus Konstruktorargumenten durch die
  // Verwendung einer Memberinitialisiererliste.
public:
  Edge(Node node1, Node node2)
    : node1(node1), node2(node2)
  {}

  Node node1;
  Node node2;
};

class CompatibilityGraph {

public:
  CompatibilityGraph(MoleculePtr mol1, MoleculePtr mol2);
  ~CompatibilityGraph();

  size_t getNofNodes();
  std::vector<Node> getNodes() const;
  Node getNode(unsigned i) const;

  bool hasEdge(const Node &a, const Node &b) const;

private:
/* Hier bitte Membervariablen zum Speichern von Knoten und Kanten einfuegen. */
  std::vector<Node> nodes;
  std::vector<Edge> edges;
};

}
