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

void graph::print(void){
  std::cout << "Graph of size " << size << std::endl;
}





