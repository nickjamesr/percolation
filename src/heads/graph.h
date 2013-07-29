// graph.h
// Header file for graph class

#ifndef h_graph
#define h_graph

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

#include <gsl/gsl_rng.h>

class vertex{
  private:
  public:
    vertex* parent;
    bool visited;
    uint distance;
    std::vector<vertex*> adj;
    // Constructors
    vertex(void);
    // Access methods
    void reset(void);
    void add(vertex*);
    void percolate(double, gsl_rng*);
};

class graph{
  private:
  protected:
    uint size;
  public:
    vertex* adj;
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
    void bfs(vertex*);
    void print(void) const;
};

#endif
