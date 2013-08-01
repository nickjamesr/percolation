// main.cc
// Testing and stuff goes in here

#include <iostream>
#include <string>

#include "heads/graph.h"
#include "heads/lattice.h"
#include "heads/main.h"

int main(int argc, char** argv){
  return test(argc, argv);
}

int test(int argc, char** argv){
  lattice_t c = lattices::cubic();
  lattice L,M;

  L = lattice(c,1,1,2);
  L.print();

  M = L;
  M.print();

  lattice N=M;
  N.print();

  return 0;
}

int run(int argc, char** argv){
  uint x=2,y=2,z=2;
  int nreps=1;
  double p=0.5;
  vertex* v;
  lattice_t c = lattices::raussendorf();
  lattice L;

  for (int i=0; i<nreps; i++){
    L = lattice(c, x, y, z);
    L.percolate(p, 314+592*i);
    v = L.traverse();
    std::cout << v->distance << std::endl;
  }

  return 0;  
}

int demo(int argc, char** argv){
  uint x=10,y=10,z=10;
  lattice L(lattices::cubic(),x,y,z); // Creates a lattice of cubic type
                                         // (also allowed are raussendorf and 
                                         // diamond) of dimension 10x10x20 unit
                                         // cells
  L.percolate(0.2); // Percolates the lattice with probability 0.5 of *forming*
                    // bonds
  vertex* out = L.traverse(); // Traverses the lattice, finding the shortest
                              // path from the z=0 plane to the z=19 plane.
                              // Returns the end vertex corresponding to that
                              // shortest path.
  std::cout << out-L.adj // Print the index of that vertex
  << " (" << out->distance << ")" << std::endl; // Print the number of steps
                                                // required to reach that vertex
  L.trace(out); // Print the path taken to get there

  //for (uint i=0; i<x*y*z; i++){
    //std::cout << i << ": " << L.adj[i].distance << std::endl;
  //}

  return 0;
}
