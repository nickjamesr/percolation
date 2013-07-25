// main.cc
// Testing and stuff goes in here

#include <iostream>
#include <string>

#include "heads/graph.h"
#include "heads/lattice.h"
#include "heads/main.h"

int main(int argc, char** argv){
  lattice L(lattices::raussendorf(), 50, 50, 50);
  L.print();
  L.percolate(0.5);
  L.print();

  return 0;
}
