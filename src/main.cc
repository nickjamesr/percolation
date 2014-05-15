// main.cc
// Testing and stuff goes in here

#include <iostream>
#include <fstream>
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
  uint dim=3;
  lattice L(c, dim, dim, dim);
  L.bfs((uint)0, 0);
  L.print();
  return 0;
}

int run(int argc, char** argv){
  lattice_t c = lattices::diamond();
  lattice L;
  uint nreps=5000, dim=8, seed=314,
    size1d, size2d, size3d,
    n1d=0, n2d=0, n3d=0,
    sumsizes1d=0, sumsizes2d=0, sumsizes3d=0;
  double pmin=0.2, pmax=0.6, pincr=0.005,
    mean1d, mean2d, mean3d, p1d, p2d, p3d;
  std::vector<uint> minsizes;
  std::ofstream fout("out.dat");
  gsl_rng *r=gsl_rng_alloc(gsl_rng_mt19937);

  if (argc>1){
    seed=atoi(argv[1]);
  }
  gsl_rng_set(r, seed);

  std::cout << "# " << dim << "x" << dim << "x" << dim << " " << c.label <<
    " lattice" << std::endl;
  std::cout << "# " << nreps << " trials per point" << std::endl;
  std::cout << "# " << "seed " << seed << std::endl;
  std::cout << "# p p_x1d p_x2d p_x3d <l_1d> <l_2d> <l_3d>" << std::endl;

  fout << "# " << dim << "x" << dim << "x" << dim << " " << c.label << 
    " lattice" << std::endl;
  fout << "# " << nreps << " trials per point" << std::endl;
  fout << "# " << "seed " << seed << std::endl;
  fout << "# p p_x1d p_x2d p_x3d <l_1d> <l_2d> <l_3d>" << std::endl;

  for (double p=pmin; p<(pmax+pincr/2.); p+=pincr){
    sumsizes1d=sumsizes2d=sumsizes3d=0;
    n1d=n2d=n3d=0;
    for (uint i=0; i<nreps; i++){
      L = lattice(c,dim,dim,dim);
      L.percolate(p, gsl_rng_get(r));
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
  
    std::cout << p << " " << p1d << " " << p2d << " " << p3d << " " <<
      mean1d << " " << mean2d << " " << mean3d << std::endl;
    fout << p << " " << p1d << " " << p2d << " " << p3d << " " <<
      mean1d << " " << mean2d << " " << mean3d << std::endl;
  }

  fout.close();

  return 0;
}

