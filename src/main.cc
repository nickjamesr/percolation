// main.cc
// Testing and stuff goes in here

#include <iostream>
#include <string>
#include <gsl/gsl_rng.h>
#include <curses.h>

#include "heads/graph.h"
#include "heads/lattice.h"
#include "heads/main.h"

int main(int argc, char** argv){
  return test(argc, argv);
}

int test(int argc, char** argv){
  lattice_t c = lattices::cubic();
  lattice L;
  uint nreps=1000, dim=10, seed=314,
    size1d, size2d, size3d,
    n1d=0, n2d=0, n3d=0,
    sumsizes1d=0, sumsizes2d=0, sumsizes3d=0;
  double p=0.3, mean1d, mean2d, mean3d, p1d, p2d, p3d;
  std::vector<uint> minsizes;
  if (argc>1){
    p=atof(argv[1]);
  }

  for (uint i=0; i<nreps; i++){
    L = lattice(c,dim,dim,dim);
    L.percolate(p, seed+i);
    L.traverse();
    minsizes=L.findCrossings();

    size1d=std::min(minsizes[0],std::min(minsizes[1],minsizes[2]));
    size2d=std::min(minsizes[3],std::min(minsizes[4],minsizes[5]));
    size3d=minsizes[6];

    if (size1d != (uint)-1){
      sumsizes1d+=size1d;
      n1d++;
    }
    if (size2d != (uint)-1){
      sumsizes2d+=size2d;
      n2d++;
    }
    if (size3d != (uint)-1){
      sumsizes3d+=size3d;
      n3d++;
    }
  }

  p1d=n1d/(double)nreps;
  p2d=n2d/(double)nreps;
  p3d=n3d/(double)nreps;
  mean1d=sumsizes1d/(double)n1d;
  mean2d=sumsizes2d/(double)n2d;
  mean3d=sumsizes3d/(double)n3d;
  
  std::cout << dim << "x" << dim << " lattice" << std::endl <<
    "Success probabilitiies: " << p1d << " " << p2d << " " << p3d <<
    std::endl <<
    "Mean sizes:             " << mean1d << " " << mean2d << " " << mean3d <<
    std::endl;

  return 0;
}

