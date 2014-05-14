/* graph.cc
 * Graph class
 * - Base class for graphs
 * - No knowledge of underlying structure of graph
 * - Adjacency table representation (less suitable for very dense graphs)
 */

#include "heads/graph.h"

//--------------------VERTEX CLASS--------------------------------------------//

graph::vertex::vertex(void){
/* Empty constructor for vertex object. Initialises to completely unconnected
 * state with bfs variables set to defaults.
 */
  for (uint i=0; i<6; i++){
    parent[i] = this;
    visited[i] = false;
    distance[i] = -1;
    clusterid[i] = 0;
  }
}

void graph::vertex::percolate(double q, gsl_rng* r){
/* Delete outgoing edges probabilistically with probability q.
 * If an edge to vertex v is deleted, this method dereferences the pointer and
 * also deletes the edge from that vertex.
 * q : Probability of deletion
 * r : (initialised) GSL random number generator.
 */
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
/* Reset vertex to its default state.
 * This resets all the bfs variables to: self-parent, not visited, distance
 * -1, cluster id 0.
 * Adjacency list is not affected, since the graph object has no default
 * connectivity.
 */
  for (uint i=0; i<6; i++){
    parent[i] = this;
    visited[i] = false;
    distance[i] = -1;
    clusterid[i] = 0;
  }
}

//--------------------GRAPH CLASS---------------------------------------------//

graph::graph(void){
/* Empty constructor for graph class.
 * Creates graph with zero vertices and no adjacency
 */
  size = 0;
  adj = new vertex[size];
}

graph::graph(uint n){
/* Size constructor for graph class
 * Creates a graph with n vertices and no adjacency
 */
  size = n;
  adj = new vertex[size];
  reset();
}

graph::graph(const graph& G){
/* Copy constructor for graph class
 * Creates a new graph as a copy of graph G
 */
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
/* Destructor for graph class
 * Makes sure memory for array of vertices is freed.
 */
  delete[] adj;
}

graph graph::operator=(const graph &G){
/* Assignment operator
 * Assigns state of graph object to that of graph G. Returns self.
 */
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
/* Reset graph to original state. Just performs a reset on vertices. Cannot
 * change the adjacency since there is no default for a graph object.
 */
  vertex *v;
  for (uint i=0; i<size; i++){
    v=adj+i;
    v->reset();
  }
}

void graph::percolate(double p, uint seed){
/* Percolate the graph: probabilistically remove edges.
 * p    : probability of forming bonds.
 * seed : seed value for rng
 */
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
 * and labelling in direction dir (optionally tagging with number id)
 * start : index of starting vertex
 * dir   : direction (0,1,...,5) This affects which values to update in
 *         clusterid, visited and distance. Naive support for directionality
 * id    : id to use for this connected component
 */
  vertex *v=adj+start;
  std::queue<vertex*> Q;
  if (v->visited[dir])
    return; // Already visited on a previous bfs
  v->parent[dir] = v;
  v->distance[dir] = 0;
  v->visited[dir] = true;
  v->clusterid[dir]=id;
  Q.push(v);
  bfs(&Q, dir, id);
}

void graph::bfs(std::queue<vertex*> *Q, uint dir, uint id){
/* Breadth-first search over graph, starting from queued vertices
 * and labelling in direction dir (optionally tagging with number id)
 * Q    : Initially queued vertices
 * dir  : direction (0,1,...,5) This affects which values to update in
 *        clusterid, visited and distance. Naive support for directionality
 * id   : id to use for this connected component
 */
  vertex *v, *u;
  while (!Q->empty()){
    v = Q->front();
    Q->pop();
    for (uint i=0; i<v->adj.size(); i++){
      u = v->adj[i];
      if (!u->visited[dir]){
        u->parent[dir] = v;
        u->visited[dir] = true;
        u->distance[dir] = v->distance[dir]+1;
        u->clusterid[dir]=id;
        Q->push(u);
      }
    }
  }
}

void graph::print(void) const{
/* Print summary of graph to cout
 */
  std::cout << "Graph of size " << size << std::endl;
}





