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
  //return run(argc, argv);
  return test(argc, argv);
  //return demo(argc, argv);
}

int test(int argc, char** argv){
  lattice_t c = lattices::diamond();
  lattice L;
  uint dim=2, seed=314;
  double p=0.6;
  if (argc>1){
    p=atof(argv[1]);
  }
  L = lattice(c,dim,dim,dim);
  L.percolate(p, seed);
  for (uint i=0; i<32; i++){
    L.bfs(i,0,i+1);
    L.bfs(i,1,i+1);
    L.bfs(i,2,i+1);
  }
  L.print();

  return 0;
}

int run(int argc, char** argv){
  gsl_rng* r=gsl_rng_alloc(gsl_rng_mt19937);
  uint seed=314;
  if (argc>1){
    seed=atoi(argv[1]);
  }
  gsl_rng_set(r, seed);
  double p=0.45;
  uint v, end=0, dim=6, sum=0;
  int progress=2, nreps=100000;
  lattice_t c = lattices::diamond();
  lattice L;

  // Set up curses stuff for progress bar
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  printw("[");
  mvprintw(0,51,"]");
  addch('0');
  addch('%');
  refresh();

  for (int i=0; i<nreps; i++){
    while (100*i/(double)nreps>progress){
      mvprintw(0,progress/2,"#");
      mvprintw(0,52,"%d",progress);
      addch('%');
      progress += 2;
      refresh();
    }
    L = lattice(c,dim,dim,dim);
    L.percolate(p, gsl_rng_get(r));
    v = L.traverse_y();
    if (L.distance(v)==(uint)-1){
      end += 1;
    }
    else{
      sum += L.distance(v)-2;
    }
  }
  endwin();

  std::cout << c.label << " lattice of dimension " << dim << "x" << dim << "x" 
    << dim << " with probability " << p << std::endl;
  std::cout << end << "/" << nreps << " failed (" <<
    end/(double)nreps <<
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
