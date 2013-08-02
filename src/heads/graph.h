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


class graph{
  private:
  protected:
    uint size;
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
  public:
    vertex* adj;
    // Constructors
    graph(void);
    graph(uint);
    graph(const graph&);
    // Destructor
    ~graph(void);
    // Overloads
    graph operator=(const graph&);
    // Access methods
    void percolate(double, uint seed=314);
    void bfs(uint);
    uint distance(uint);
    uint parent(uint);
    void print(void) const;
};

#endif
