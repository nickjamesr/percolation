// main.cc
// Testing and stuff goes in here

#include <iostream>
#include <string>
#include <gsl/gsl_rng.h>

#include "heads/graph.h"
#include "heads/lattice.h"
#include "heads/main.h"

int main(int argc, char** argv){
  return run(argc, argv);
  //return test(argc, argv);
}

int test(int argc, char** argv){
  lattice_t c = lattices::diamond();
  lattice L;
  uint dim=2;

  for (uint i=0; i<2; i++){
    L = lattice(c,dim,dim,dim);
    L.percolate(0.5);
  }

  return 0;
}

int run(int argc, char** argv){
  gsl_rng* r=gsl_rng_alloc(gsl_rng_mt19937);
  double p=0.5, progress=0.02;
  uint v, end=0, dim=6, sum=0;
  int nreps=100000;
  lattice_t c = lattices::diamond();
  lattice L;

  for (int i=0; i<nreps; i++){
    while (i/(double)nreps>progress){
      std::cout << progress*100 << "%" << std::endl;
      progress += 0.02;
    }
    L = lattice(c,dim,dim,dim+2);
    L.percolate(p, gsl_rng_get(r));
    v = L.traverse();
    if (L.distance(v)==(uint)-1){
      end += 1;
    }
    else{
      sum += L.distance(v)-2;
    }
  }
  std::cout << c.label << " lattice of dimension " << dim << "x" << dim << "x" 
    << dim << " with probability " << p << std::endl;
  std::cout << end << "/" << nreps << " failed (" << end/(double)nreps <<
    ")" << std::endl;
  std::cout << "Average length: " << sum/(double)(nreps-end) << std::endl;

  gsl_rng_free(r);
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
