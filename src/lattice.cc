// lattice.cc
// lattice and lattice_t classes, plus some definitions for lattice types
// - Derived from graph
// - Contains information about the unit cell and heigh, width, depth of lattice

# include "heads/lattice.h"

lattice::lattice(void){
  dimx = 0;
  dimy = 0;
  dimz = 0;
  size = dimx*dimy*dimz;
  adjacency = new std::vector<uint>[size];
  type = lattice_t();
}

lattice::lattice(const lattice& lat) : graph(lat.size){
  dimx = lat.dimx;
  dimy = lat.dimy;
  dimz = lat.dimz;
  size = lat.size;
  type = lat.type;
  for (int i=0; i<size; i++){
    adjacency[i] = lat.adjacency[i];
  }
}

lattice::lattice(lattice_t D, uint L, uint M, uint N) : graph(L*M*N){
  dimx = L;
  dimy = M;
  dimz = N;
  size = L*M*N;
  type = D;
}

lattice::~lattice(void){
}

lattice lattice::operator=(const lattice lat){
  delete[] adjacency;
  size = lat.size;
  dimx = lat.dimx;
  dimy = lat.dimy;
  dimz = lat.dimz;
  type = lat.type;
  adjacency = new std::vector<uint>[size];
  for (int i=0; i<size; i++){
    adjacency[i] = lat.adjacency[i];
  }
  return *this;
}

void lattice::print(void){
  std::cout << type.label << " lattice of size " << dimx << " x " << dimy <<
    " x " << dimz << std::endl;
}

// lattice_t methods

lattice_t::coord::coord(int hh, int ii, int jj, int kk){
  h = hh;
  i = ii;
  j = jj;
  k = kk;
}

lattice_t::coord::coord(const lattice_t::coord& C){
  h = C.h;
  i = C.i;
  j = C.j;
  k = C.k;
}

lattice_t::lattice_t(void){
  size = 0;
  adjacency = new std::vector<coord>[size];
  label = "null";
}

lattice_t::lattice_t(const lattice_t& D){
  size = D.size;
  adjacency = new std::vector<coord>[size];
  for (uint i=0; i<size; i++){
    adjacency[i] = D.adjacency[i];
  }
  label = D.label;
}

lattice_t::lattice_t(uint n, std::string s){
  size = n;
  adjacency = new std::vector<coord>[size];
  label = s;
}

lattice_t::~lattice_t(void){
  delete[] adjacency;
}

lattice_t lattice_t::operator=(const lattice_t D){
  delete[] adjacency;
  size = D.size;
  adjacency = new std::vector<coord>[size];
  for (uint i=0; i<size; i++){
    adjacency[i] = D.adjacency[i];
  }
  label = D.label;
  return *this;
}

void lattice_t::add(uint start, int h, int i, int j, int k){
  lattice_t::coord C(h,i,j,k);
  adjacency[start].push_back(C);
}

void lattice_t::print(void){
  std::cout << label << " lattice\n" << "unit cell of size " << size <<
    " with the following connections:" << std::endl;
  for (uint i=0; i<size; i++){
    std::vector<lattice_t::coord> v = adjacency[i];
    int n = v.size();
    std::cout << "vertex " << i << std::endl;
    for (int j=0; j<n; j++){
      std::cout << "  (" << v[j].h << "," << v[j].i << "," << v[j].j << "," <<
        v[j].k << ")" << std::endl;
    }
  }
}

// Generator functions

lattice_t lattices::cubic(void){
  lattice_t D(1, "cubic");
  D.add(0,0,-1,0,0);
  D.add(0,0,+1,0,0);
  D.add(0,0,0,-1,0);
  D.add(0,0,0,+1,0);
  D.add(0,0,0,0,-1);
  D.add(0,0,0,0,+1);
  return D;
}

lattice_t lattices::raussendorf(void){
  lattice_t D(6, "raussendorf");
  D.add(0,1,0,0,0);
  D.add(0,5,0,0,0);
  D.add(0,1,0,-1,0);
  D.add(0,5,0,0,-1);

  D.add(1,2,0,0,0);
  D.add(1,0,0,0,0);
  D.add(1,2,-1,0,0);
  D.add(1,0,0,1,0);

  D.add(2,3,0,0,0);
  D.add(2,1,0,0,0);
  D.add(2,3,0,0,-1);
  D.add(2,1,1,0,0);

  D.add(3,4,0,0,0);
  D.add(3,2,0,0,0);
  D.add(3,4,0,1,0);
  D.add(3,2,0,0,1);

  D.add(4,5,0,0,0);
  D.add(4,3,0,0,0);
  D.add(4,5,1,0,0);
  D.add(4,3,0,-1,0);

  D.add(5,0,0,0,0);
  D.add(5,4,0,0,0);
  D.add(5,0,0,0,1);
  D.add(5,4,-1,0,0);
  return D;
}

lattice_t lattices::diamond(void){
  lattice_t D(8, "diamond");
  D.add(0,1,0,0,0);
  D.add(0,5,-1,1,0);
  D.add(0,6,-1,0,1);
  D.add(0,7,0,1,1);

  D.add(1,0,0,0,0);
  D.add(1,2,0,0,0);
  D.add(1,3,0,0,0);
  D.add(1,4,0,0,0);

  D.add(2,1,0,0,0);
  D.add(2,5,0,0,0);
  D.add(2,6,0,0,1);
  D.add(2,7,0,0,1);

  D.add(3,1,0,0,0);
  D.add(3,5,0,1,0);
  D.add(3,6,0,0,0);
  D.add(3,7,0,1,0);

  D.add(4,1,0,0,0);
  D.add(4,5,-1,0,0);
  D.add(4,6,-1,0,0);
  D.add(4,7,0,0,0);

  D.add(5,0,1,-1,0);
  D.add(5,2,0,0,0);
  D.add(5,3,0,-1,0);
  D.add(5,4,1,0,0);
  
  D.add(6,0,1,0,-1);
  D.add(6,2,0,0,1);
  D.add(6,3,0,0,0);
  D.add(6,4,1,0,0);
  
  D.add(7,0,0,-1,-1);
  D.add(7,2,0,0,-1);
  D.add(7,3,0,-1,0);
  D.add(7,4,0,0,0);
  return D;
}





