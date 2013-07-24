// main.cc
// Testing and stuff goes in here

#include <iostream>
#include <string>

#include "heads/graph.h"
#include "heads/lattice.h"
#include "heads/main.h"

int main(int argc, char** argv){
  lattice_t D = lattices::cubic();
  D.print();
  D = lattices::raussendorf();
  D.print();
  lattice_t T(lattices::diamond());
  T.print();

  return 0;
}
