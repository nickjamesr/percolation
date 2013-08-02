// main.cc
// Testing and stuff goes in here

#include <iostream>
#include <string>

#include "heads/graph.h"
#include "heads/lattice.h"
#include "heads/main.h"

int main(int argc, char** argv){
  return run(argc, argv);
}

int run(int argc, char** argv){
  uint dim=20;
  int nreps=20;
  //double p=0.6;
  uint v;
  uint sum=0;
  lattice_t c = lattices::diamond();
  lattice L;

  for (double p=0.4; p<0.6; p+=0.02){
  sum=0;
  for (int i=0; i<nreps; i++){
    L = lattice(c,dim,dim,dim);
    L.percolate(p, 314+592*i);
    v = L.traverse();
    sum += L.distance(v);
    //std::cout << L.distance(v) << std::endl;
  }
  std::cout << p << " " << dim << " " << sum/(double)nreps << std::endl;
  }

  return 0;  
}

int demo(int argc, char** argv){
  uint x=50,y=50,z=50;
  lattice L(lattices::cubic(),x,y,z); // Creates a lattice of cubic type
                                      // (also allowed are raussendorf and 
                                      // diamond) of dimension (x,y,z) unit
                                      // cells
  L.percolate(0.5); // Percolates the lattice with probability 0.5 of *forming*
                    // bonds
  uint out = L.traverse(); // Traverses the lattice, finding the shortest
                           // path from the z=0 plane to the z=19 plane.
                           // Returns the end vertex corresponding to that
                           // shortest path.
  std::cout << out // Prints the index of that vertex
  << " (" << L.distance(out) << ")" << std::endl; // Print the number of steps
                                                  // required to reach that
                                                  // vertex
  L.trace(out); // Print the path taken to get there

  return 0;
}
