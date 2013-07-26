// main.cc
// Testing and stuff goes in here

#include <iostream>
#include <string>

#include "heads/graph.h"
#include "heads/lattice.h"
#include "heads/main.h"

int main(int argc, char** argv){
  lattice L(lattices::cubic(),2,3,3);
  L.percolate(0.5);
  L.print();
  L.bfs();
  for (uint i=0; i<18; i++){
    std::cout << L.adj[i].distance << std::endl;
  }

  return 0;
}
