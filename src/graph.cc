// graph.cc
// Graph class
// - Base class for graphs
// - No knowledge of underlying structure of graph
// - Adjacency table representation (less suitable for very dense graphs)

#include "heads/graph.h"

// Vertex class

graph::vertex::vertex(void){
  parent = this;
  visited = false;
  distance = -1;
}

void graph::vertex::add(vertex* v){
  adj.push_back(v);
}

void graph::vertex::percolate(double q, gsl_rng* r){
  // Cycle through edges deleting with probability q
  vertex* in, *out;
  uint j;
  for (uint i=0; i<adj.size(); i++){
    if (gsl_rng_uniform(r) < q){
      out = adj[i];
      adj.erase(adj.begin()+i);
      i--;
      // Erase incoming connection
      j = 0;
      do{
        in = out->adj[j];
        j++;
      } while (in != this);
      out->adj.erase(out->adj.begin()+j-1);
    }
  }
}

void graph::vertex::reset(void){
  parent = this;
  visited = false;
  distance = -1;
}

// Graph class

graph::graph(void){
  size = 0;
  adj = new vertex[size];
}

graph::graph(uint i){
  size = i;
  adj = new vertex[size];
}

graph::graph(const graph& G){
  size = G.size;
  adj = new vertex[size];
  vertex *u, *v;
  uint n;
  for (uint i=0; i<size; i++){
    // Copy the vertex manually
    u = adj+i;
    v = G.adj+i;
    u->visited = v->visited;
    u->distance = v->distance;
    u->parent = u+(v->parent-v);
    n = v->adj.size();
    for (uint j=0; j<n; j++){
      u->add(u+(v->adj[j]-v)); // Retain relative positions
    }
  }
}

graph::~graph(void){
  delete[] adj;
}

graph graph::operator=(const graph &G){
  delete[] adj;
  size = G.size;
  adj = new vertex[size];
  vertex *u, *v;
  uint n;
  for (uint i=0; i<size; i++){
    // Copy the vertex manually
    u = adj+i;
    v = G.adj+i;
    u->visited = v->visited;
    u->distance = v->distance;
    u->parent = u+(v->parent-v);
    n = v->adj.size();
    for (uint j=0; j<n; j++){
      u->add(u+(v->adj[j]-v)); // Retain relative positions
    }
  }
  return *this;
}

void graph::percolate(double p, uint seed){
  double q = 1-std::sqrt(p);
  gsl_rng* r = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(r, seed);
  for (uint i=0; i<size; i++){
    adj[i].percolate(q, r);
  }
  gsl_rng_free(r);
}

void graph::bfs(uint i){
  vertex *v=adj+i;
  std::queue<vertex*> Q;
  // Reset all vertices
  for (uint i=0; i<size; i++){
    adj[i].reset();
  }
  // Decide on a starting point (need a better way of doing this!)
  vertex* u;
  v->parent = v;
  v->distance = 0;
  v->visited = true;
  Q.push(v);
  while (!Q.empty()){
    v = Q.front();
    Q.pop();
    for (uint i=0; i<v->adj.size(); i++){
      u = v->adj[i];
      if (!u->visited){
        u->parent = v;
        u->visited = true;
        u->distance = v->distance+1;
        Q.push(u);
      }
    }
  }
}

uint graph::parent(uint i){
  return (adj[i].parent-adj);
}

uint graph::distance(uint i){
  return adj[i].distance;
}

void graph::print(void) const{
  std::cout << "Graph of size " << size << std::endl;
}





