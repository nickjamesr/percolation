// graph.h
// Header file for graph class

#ifndef h_graph
#define h_graph

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#include <gsl/gsl_rng.h>

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
    void percolate(double, uint seed=314);
    void print(void) const;
};

#endif
