// lattice.cc
// lattice and lattice_t classes, plus some definitions for lattice types
// - Derived from graph
// - Contains information about the unit cell and heigh, width, depth of lattice

# include "heads/lattice.h"

lattice::lattice(void){
  // Empty constructor (never really used but I think it's necessary)
  dimx = 0;
  dimy = 0;
  dimz = 0;
  size = 0;
  adj = new vertex[size];
  type = lattice_t();
}

lattice::lattice(const lattice& lat) : graph(lat.size){
  // Copy constructor with deep copy of adjacency list
  dimx = lat.dimx;
  dimy = lat.dimy;
  dimz = lat.dimz;
  type = lat.type;
}

lattice::lattice(lattice_t D, uint L, uint M, uint N) : graph(L*M*N*D.size){
  // Constructs an LxMxN lattice from the unit cell D
  dimx = L;
  dimy = M;
  dimz = N;
  type = D;
  uint n=0;
  uint connect=0;
  int outw, outx, outy, outz;
  for (iterator I(D.size, dimx, dimy, dimz); I<size; I++){
    n = I.index();
    for (uint i=0; i<D.adjacency[I[0]].size(); i++){
      outw = D.geth(I[0],i);
      outx = I[1] + D.geti(I[0],i);
      outy = I[2] + D.getj(I[0],i);
      outz = I[3] + D.getk(I[0],i);
      if (outx >= 0 && outx < (int)dimx &&
          outy >= 0 && outy < (int)dimy &&
          outz >= 0 && outz < (int)dimz){
        connect = fromCoord(outw, outx, outy, outz);
        adj[n].add(adj+connect);
      }
    }
  }
}

// Desctructor
lattice::~lattice(void){
}

// Assignment operator with deep copy of adjacency list
lattice lattice::operator=(const lattice lat){
  delete[] adj;
  size = lat.size;
  dimx = lat.dimx;
  dimy = lat.dimy;
  dimz = lat.dimz;
  type = lat.type;
  adj= new vertex[size];
  for (uint i=0; i<size; i++){
    adj[i] = lat.adj[i];
  }
  return *this;
}

// Helper function for converting 4-position into global index
uint lattice::fromCoord(int h, int i, int j, int k){
  return h + type.size*(i + dimx*(j + dimy* k));
}

// Currently just a brief summary of the lattice properties
void lattice::print(void){
  std::cout << type.label << " lattice of size " << dimx << " x " << dimy <<
    " x " << dimz << std::endl;
  int n, from;
  vertex* to;
  for (iterator I(type.size, dimx, dimy, dimz); I<size; I++){
    from = I.index();
    n = adj[from].adj.size();
    for (int i=0; i<n; i++){
      to = adj[from].adj[i];
      std::cout << from << " -> " << to-adj << std::endl;
    }
    std::cout << std::endl;
  }
}

// Nested class for iterating through lattices
lattice::iterator::iterator(){
  size[0] = size[1] = size[2] = size[3] = 0;
  n = 0;
  coord[0] = coord[1] = coord[2] = coord[3] = 0;
}

lattice::iterator::iterator(uint dimw, uint dimx, uint dimy, uint dimz){
  size[0] = dimw;
  size[1] = dimx;
  size[2] = dimy;
  size[3] = dimz;
  n = 0;
  coord[0] = coord[1] = coord[2] = coord[3] = 0;
}

void lattice::iterator::operator++(int){
  n++;
  if (coord[0]<size[0]-1){
    coord[0]++;
  }
  else if (coord[1]<size[1]-1){
    coord[0] = 0;
    coord[1]++;
  }
  else if (coord[2]<size[2]-1){
    coord[0] = 0;
    coord[1] = 0;
    coord[2]++;
  }
  else if (coord[3]<size[3]-1){
    coord[0] = 0;
    coord[1] = 0;
    coord[2] = 0;
    coord[3]++;
  }
  else{
    coord[0] = coord[1] = coord[2] = coord[3] = 0;
  }
}

uint lattice::iterator::operator[](uint i) const{
  if (i<4){
    return coord[i];
  }
  else{
    return 0;
  }
}

bool lattice::iterator::operator<(uint i) const{
  return (n<i);
}

bool lattice::iterator::operator>(uint i) const{
  return (n>i);
}

bool lattice::iterator::operator<=(uint i) const{
  return (n<=i);
}

bool lattice::iterator::operator>=(uint i) const{
  return (n>=i);
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





