// graph.h
// Header file for graph class

#ifndef h_graph
#define h_graph

#include <cstdlib>
#include <iostream>
#include <vector>

class graph{
  private:
  protected:
    uint size;
    std::vector<uint>* adjacency;
  public:
    // Constructors
    graph(void);
    graph(uint);
    graph(const graph&);
    // Destructor
    ~graph(void);
    // Overloads
    graph operator=(graph);
    // Access methods
    void print(void);
};

#endif
