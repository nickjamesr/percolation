/* lattice.cc
 * lattice and lattice_t classes, plus some definitions for lattice types
 * - Derived from graph
 * - Contains information about the unit cell and height, width, depth of
 *   lattice
 */

# include "heads/lattice.h"

//--------------------LATTICE METHODS-----------------------------------------//

lattice::lattice(void){
/* Empty constructor
 */
  dimx = 0;
  dimy = 0;
  dimz = 0;
  size = 0;
  type = lattice_t();
}

lattice::lattice(const lattice& lat) : graph(lat.size){
  // Copy constructor with deep copy of adjacency list
  dimx = lat.dimx;
  dimy = lat.dimy;
  dimz = lat.dimz;
  type = lat.type;
  //adj = new vertex[size];
  vertex *u, *v;
  uint n;
  for (uint i=0; i<size; i++){
    // Copy the vertex manually
    u = adj+i;
    v = lat.adj+i;
    for (uint j=0; j<6; j++){
      u->visited[j] = v->visited[j];
      u->distance[j] = v->distance[j];
      u->parent[j] = u+(v->parent[j]-v);
      u->clusterid[j] = v->clusterid[j];
    }
    n = v->adj.size();
    for (uint j=0; j<n; j++){
      u->add(u+(v->adj[j]-v)); // Retain relative positions
    }
  }
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
      outw = D.adjacency[I[0]][i].h;
      outx = I[1] + D.adjacency[I[0]][i].i;
      outy = I[2] + D.adjacency[I[0]][i].j;
      outz = I[3] + D.adjacency[I[0]][i].k;
      //outw = D.geth(I[0],i);
      //outx = I[1] + D.geti(I[0],i);
      //outy = I[2] + D.getj(I[0],i);
      //outz = I[3] + D.getk(I[0],i);
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
lattice lattice::operator=(const lattice &lat){
  delete[] adj;
  size = lat.size;
  dimx = lat.dimx;
  dimy = lat.dimy;
  dimz = lat.dimz;
  type = lat.type;
  adj = new vertex[size];
  vertex *u, *v;
  uint n;
  for (uint i=0; i<size; i++){
    // Copy the vertex manually
    u = adj+i;
    v = lat.adj+i;
    for (uint j=0; j<6; j++){
      u->visited[j] = v->visited[j];
      u->distance[j] = v->distance[j];
      u->parent[j] = u+(v->parent[j]-v);
      u->clusterid[j]=v->clusterid[j];
    }
    n = v->adj.size();
    for (uint j=0; j<n; j++){
      u->add(u+(v->adj[j]-v)); // Retain relative positions
    }
  }
  return *this;
}

// BFS-type stuff
void lattice::traverse(){
/* Find the connected clusters of the lattice by doing successive traverses in
 * the positive and negative x, y and z directions.
 * After running, each vertex in the lattice will have 6 indices (one for each
 * direction) indicating which clusters it is in.
 * More processing is required to find which (if any) of these are crossing
 * clusters
 */
  uint i,j,k,idx;
  std::queue<vertex*> Q;
  vertex *v;

// +x direction
  i=0;
  idx=1;
  for (k=0; k<dimz; k++){
    for (j=0; j<dimy; j++){
      for (auto h : type.startx){
        idx=fromCoord(h,i,j,k);
        v=adj+idx;
        v->parent[0]=v;
        v->distance[0]=0;
        v->visited[0]=true;
        v->clusterid[0]=0;
        Q.push(adj+idx);
      }
    }
  }
  bfs(&Q, 0);

// +y direction
  j=0;
  idx=1;
  for (k=0; k<dimz; k++){
    for (i=0; i<dimx; i++){
      for (auto h : type.starty){
        idx=fromCoord(h,i,j,k);
        v=adj+idx;
        v->parent[1]=v;
        v->distance[1]=0;
        v->visited[1]=true;
        v->clusterid[1]=0;
        Q.push(adj+idx);
      }
    }
  }
  bfs(&Q, 1);

// +z direction
  k=0;
  idx=1;
  for (j=0; j<dimy; j++){
    for (i=0; i<dimx; i++){
      for (auto h : type.startz){
        idx=fromCoord(h,i,j,k);
        v=adj+idx;
        v->parent[2]=v;
        v->distance[2]=0;
        v->visited[2]=true;
        v->clusterid[2]=0;
        Q.push(adj+idx);
      }
    }
  }
  bfs(&Q, 2);

// -x direction
  i=dimx-1;
  idx=1;
  for (k=0; k<dimz; k++){
    for (j=0; j<dimy; j++){
      for (auto h : type.endx){
        idx=fromCoord(h,i,j,k);
        v=adj+idx;
        v->parent[3]=v;
        v->distance[3]=0;
        v->visited[3]=true;
        v->clusterid[3]=0;
        Q.push(adj+idx);
      }
    }
  }
  bfs(&Q, 3);

// -y direction
  j=dimy-1;
  idx=1;
  for (k=0; k<dimz; k++){
    for (i=0; i<dimx; i++){
      for (auto h : type.endy){
        idx=fromCoord(h,i,j,k);
        v=adj+idx;
        v->parent[4]=v;
        v->distance[4]=0;
        v->visited[4]=true;
        v->clusterid[4]=0;
        Q.push(adj+idx);
      }
    }
  }
  bfs(&Q, 4);

// -z direction
  k=dimz-1;
  idx=1;
  for (j=0; j<dimy; j++){
    for (i=0; i<dimx; i++){
      for (auto h : type.endz){
        idx=fromCoord(h,i,j,k);
        v=adj+idx;
        v->parent[5]=v;
        v->distance[5]=0;
        v->visited[5]=true;
        v->clusterid[5]=0;
        Q.push(adj+idx);
      }
    }
  }
  bfs(&Q, 5);
}

std::vector<uint> lattice::findCrossings(){
  uint len=-1;
  std::vector<uint> minsizes(7,(uint)-1);
  vertex *v;
  for (uint i=0; i<size; i++){
    v=adj+i;
    if (v->distance[0]!=(uint)-1 && v->distance[3]!=(uint)-1){
      // Find x size
      len  = v->distance[0]+v->distance[3];
      if (len < minsizes[0]){
        minsizes[0]=len;
      }
    }
    if (v->distance[1]!=(uint)-1 && v->distance[4]!=(uint)-1){
      // Find y size
      len  = v->distance[1]+v->distance[4];
      if (len < minsizes[1]){
        minsizes[1]=len;
      }
    }
    if (v->distance[2]!=(uint)-1 && v->distance[5]!=(uint)-1){
      // Find z size
      len  = v->distance[2]+v->distance[5];
      if (len < minsizes[2]){
        minsizes[2]=len;
      }
    }
    if (v->distance[0]!=(uint)-1 && v->distance[1]!=(uint)-1 &&
        v->distance[3]!=(uint)-1 && v->distance[4]!=(uint)-1){
      // Find xy size 
      len  = v->distance[0]+v->distance[1]+v->distance[3]+v->distance[4];
      if (len < minsizes[3]){
        minsizes[3]=len;
      }
    }
    if (v->distance[1]!=(uint)-1 && v->distance[2]!=(uint)-1 &&
        v->distance[4]!=(uint)-1 && v->distance[5]!=(uint)-1){
      // Find yz size 
      len  = v->distance[1]+v->distance[2]+v->distance[4]+v->distance[5];
      if (len < minsizes[4]){
        minsizes[4]=len;
      }
    }
    if (v->distance[0]!=(uint)-1 && v->distance[2]!=(uint)-1 &&
        v->distance[3]!=(uint)-1 && v->distance[5]!=(uint)-1){
      // Find zx size 
      len  = v->distance[0]+v->distance[2]+v->distance[3]+v->distance[5];
      if (len < minsizes[5]){
        minsizes[5]=len;
      }
    }
    if (v->distance[0]!=(uint)-1 && v->distance[1]!=(uint)-1 &&
        v->distance[2]!=(uint)-1 && v->distance[3]!=(uint)-1 &&
        v->distance[4]!=(uint)-1 && v->distance[5]!=(uint)-1){
      // Find xyz size
      len  = v->distance[0]+v->distance[1]+v->distance[2]+
             v->distance[3]+v->distance[4]+v->distance[5];
      if (len < minsizes[6]){
        minsizes[6]=len;
      }
    }
  }
  for (uint i=0; i<7; i++){
    if (minsizes[i] != (uint)-1){
      minsizes[i]++;
    }
  }
  return minsizes;
}

// Currently just a brief summary of the lattice properties
void lattice::print(void){
  std::cout << type.label << " lattice of size " << dimx << " x " << dimy <<
    " x " << dimz << std::endl;
  int v;
  for (iterator I(type.size, dimx, dimy, dimz); I<size; I++){
    v = I.index();
    std::cout << v << ": (" << adj[v].distance[0];
    for (uint i=1; i<6; i++){
      std::cout << "," << adj[v].distance[i];
    }
    std::cout << ")" << std::endl;
  }
}

//--------------------ITERATOR METHODS----------------------------------------//
lattice::iterator::iterator(){
/* Empty constructor.
 * Initialise iterator for a lattice of size zero, current position zero.
 */
  size[0] = size[1] = size[2] = size[3] = 0;
  n = 0;
  coord[0] = coord[1] = coord[2] = coord[3] = 0;
}

lattice::iterator::iterator(uint dimw, uint dimx, uint dimy, uint dimz){
/* Construct with maximum size dimw x dimx x dimy x dimz.
 * Initialise state to zeroth element
 */
  size[0] = dimw;
  size[1] = dimx;
  size[2] = dimy;
  size[3] = dimz;
  n = 0;
  coord[0] = coord[1] = coord[2] = coord[3] = 0;
}

void lattice::iterator::operator++(int){
/* Incrementor
 */
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

//--------------------LATTICE_T METHODS---------------------------------------//

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
  startx=D.startx;
  starty=D.starty;
  startz=D.startz;
  endx=D.endx;
  endy=D.endy;
  endz=D.endz;
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

lattice_t lattice_t::operator=(const lattice_t &D){
  delete[] adjacency;
  size = D.size;
  adjacency = new std::vector<coord>[size];
  for (uint i=0; i<size; i++){
    adjacency[i] = D.adjacency[i];
  }
  startx=D.startx;
  starty=D.starty;
  startz=D.startz;
  endx=D.endx;
  endy=D.endy;
  endz=D.endz;
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
  D.startx.push_back(0);
  D.starty.push_back(0);
  D.startz.push_back(0);
  D.endx.push_back(0);
  D.endy.push_back(0);
  D.endz.push_back(0);
  return D;
}

lattice_t lattices::raussendorf(void){
  lattice_t D(6, "raussendorf");
  D.add(0,1,0,0,0);
  D.add(0,5,0,0,0);
  D.add(0,1,0,-1,0);
  D.add(0,5,0,0,-1);
  D.starty.push_back(0);
  D.startz.push_back(0);

  D.add(1,2,0,0,0);
  D.add(1,0,0,0,0);
  D.add(1,2,-1,0,0);
  D.add(1,0,0,1,0);
  D.startx.push_back(1);
  D.endy.push_back(1);

  D.add(2,3,0,0,0);
  D.add(2,1,0,0,0);
  D.add(2,3,0,0,-1);
  D.add(2,1,1,0,0);
  D.startz.push_back(2);
  D.endx.push_back(2);

  D.add(3,4,0,0,0);
  D.add(3,2,0,0,0);
  D.add(3,4,0,1,0);
  D.add(3,2,0,0,1);
  D.endy.push_back(3);
  D.endz.push_back(3);

  D.add(4,5,0,0,0);
  D.add(4,3,0,0,0);
  D.add(4,5,1,0,0);
  D.add(4,3,0,-1,0);
  D.starty.push_back(4);
  D.endx.push_back(4);

  D.add(5,0,0,0,0);
  D.add(5,4,0,0,0);
  D.add(5,0,0,0,1);
  D.add(5,4,-1,0,0);
  D.startx.push_back(5);
  D.endz.push_back(5);
  return D;
}

lattice_t lattices::diamond(void){
  lattice_t D(8, "diamond");
  D.add(0,1,0,0,0);
  D.add(0,5,-1,1,0);
  D.add(0,6,-1,0,1);
  D.add(0,7,0,1,1);
  D.startx.push_back(0);
  D.endy.push_back(0);
  D.endz.push_back(0);

  D.add(1,0,0,0,0);
  D.add(1,2,0,0,0);
  D.add(1,3,0,0,0);
  D.add(1,4,0,0,0);

  D.add(2,1,0,0,0);
  D.add(2,5,0,0,0);
  D.add(2,6,0,0,1);
  D.add(2,7,0,0,1);
  D.endz.push_back(2);

  D.add(3,1,0,0,0);
  D.add(3,5,0,1,0);
  D.add(3,6,0,0,0);
  D.add(3,7,0,1,0);
  D.endy.push_back(3);

  D.add(4,1,0,0,0);
  D.add(4,5,-1,0,0);
  D.add(4,6,-1,0,0);
  D.add(4,7,0,0,0);
  D.startx.push_back(4);

  D.add(5,0,1,-1,0);
  D.add(5,2,0,0,0);
  D.add(5,3,0,-1,0);
  D.add(5,4,1,0,0);
  D.starty.push_back(5);
  D.endx.push_back(5);
  
  D.add(6,0,1,0,-1);
  D.add(6,2,0,0,-1);
  D.add(6,3,0,0,0);
  D.add(6,4,1,0,0);
  D.startz.push_back(6);
  D.endx.push_back(6);
  
  D.add(7,0,0,-1,-1);
  D.add(7,2,0,0,-1);
  D.add(7,3,0,-1,0);
  D.add(7,4,0,0,0);
  D.starty.push_back(7);
  D.startz.push_back(7);
  return D;
}





