// main.cc
// Testing and stuff goes in here

#include <iostream>
#include <string>

#include "heads/graph.h"
#include "heads/lattice.h"
#include "heads/main.h"

int main(int argc, char** argv){
  lattice L(lattices::diamond(), 2, 2, 2);
  L.print();

  return 0;
}
