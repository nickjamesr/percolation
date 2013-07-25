// graph.cc
// Graph class
// - Base class for graphs
// - No knowledge of underlying structure of graph
// - Adjacency table representation (less suitable for very dense graphs)

#include "heads/graph.h"

graph::graph(void){
  size = 0;
  adjacency = new std::vector<uint>[size];
}

graph::graph(uint i){
  size = i;
  adjacency = new std::vector<uint>[size];
}

graph::graph(const graph& G){
  size = G.size;
  adjacency = new std::vector<uint>[size];
}

graph::~graph(void){
  delete[] adjacency;
}

graph graph::operator=(graph G){
  delete[] adjacency;
  size = G.size;
  adjacency = new std::vector<uint>[size];
  return *this;
}

void graph::percolate(double p, uint seed){
  double q = 1-std::sqrt(p);
  int n, that;
  gsl_rng* r = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(r, seed);
  for (uint i=0; i<size; i++){
    n = adjacency[i].size();
    for (int j=0; j<n; j++){
      if (gsl_rng_uniform(r) < q){
        // Delete edge from this vertex
        that = adjacency[i][j];
        adjacency[i].erase(adjacency[i].begin()+j);
        n--;
        j--;
        // Delete edge from that vertex
        for (uint k=0; k<adjacency[that].size(); k++){
          if (adjacency[that][k] == i){
            adjacency[that].erase(adjacency[that].begin()+k);
            break;
          }
        }
      }
    }
  }
  gsl_rng_free(r);
}

void graph::print(void) const{
  std::cout << "Graph of size " << size << std::endl;
}





