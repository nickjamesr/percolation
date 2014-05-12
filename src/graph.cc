// graph.cc
// Graph class
// - Base class for graphs
// - No knowledge of underlying structure of graph
// - Adjacency table representation (less suitable for very dense graphs)

#include "heads/graph.h"

// Vertex class

graph::vertex::vertex(void){
  for (uint i=0; i<6; i++){
    parent[i] = this;
    visited[i] = false;
    distance[i] = -1;
    clusterid[i] = 0;
  }
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
  for (uint i=0; i<6; i++){
    parent[i] = this;
    visited[i] = false;
    distance[i] = -1;
    clusterid[i] = 0;
  }
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
    for (uint i=0; i<6; i++){
      u->visited[i] = v->visited[i];
      u->distance[i] = v->distance[i];
      u->parent[i] = u+(v->parent[i]-v);
      u->clusterid[i] = v->clusterid[i];
    }
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
    for (uint i=0; i<6; i++){
      u->visited[i] = v->visited[i];
      u->distance[i] = v->distance[i];
      u->parent[i] = u+(v->parent[i]-v);
      u->clusterid[i] = v->clusterid[i];
    }
    n = v->adj.size();
    for (uint j=0; j<n; j++){
      u->add(u+(v->adj[j]-v)); // Retain relative positions
    }
  }
  return *this;
}

void graph::reset(){
  vertex *v;
  for (uint i=0; i<size; i++){
    v=adj+i;
    v->reset();
  }
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

void graph::bfs(uint start, uint dir, uint id){
/* Breadth-first search over graph, starting from the vertex with index start
 * and labelling in direction dir with number id
 * start : index of starting vertex
 * dir   : direction (0,1,...,5) This affects which values to update in
 *         clusterid, visited and distance. Naive support for directionality
 * id    : id to use for this connected component
 */
  vertex *v=adj+start, *u;
  std::queue<vertex*> Q;
  if (v->visited[dir])
    return; // Already visited on a previous bfs
  v->parent[dir] = v;
  v->distance[dir] = 0;
  v->visited[dir] = true;
  v->clusterid[dir]=id;
  Q.push(v);
  while (!Q.empty()){
    v = Q.front();
    Q.pop();
    for (uint i=0; i<v->adj.size(); i++){
      u = v->adj[i];
      if (!u->visited[dir]){
        u->parent[dir] = v;
        u->visited[dir] = true;
        u->distance[dir] = v->distance[dir]+1;
        u->clusterid[dir]=id;
        Q.push(u);
      }
    }
  }
}

uint graph::parent(uint i, uint dir){
  return (adj[i].parent[dir]-adj);
}

uint graph::distance(uint i, uint dir){
  return (adj[i].distance)[dir];
}

void graph::print(void) const{
  std::cout << "Graph of size " << size << std::endl;
}





