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
/* graph class
 * Just an array of vertex (subclass) objects with no information about e.g.
 * lattice geometry. Derived class lattice (seperate header file) provides
 * this functionality
 */
  private:
  protected:
    uint size;
    class vertex{
    /* vertex class
     * Stores a vector of pointers to adjacent vertices. Supports attributes
     * and methods to genrate the lattice and perform percolation and bfs
     * algorithms.
     */
    private:
      public:
        vertex* parent[6];        // Parent vertex in bfs. Allows directions
        uint clusterid[6];        // Optional cluster id for bfs
        bool visited[6];          // Track whether vertex has been visited
        uint distance[6];         // Distance from start of bfs
        std::vector<vertex*> adj; // Vector of (pointers to) adjacent vertices
        // Constructors
        vertex(void);
        // Access methods
        void reset(void);
          // Reset vertex to its default state
        void add(vertex* v){adj.push_back(v);};
          // Add a connection to the vertex
        void percolate(double q, gsl_rng* r);
          // Delete edges probabilistically with probability q
    };
  public:
    vertex* adj;
// Constructors
    graph(void);            // Create graph with zero vertices
    graph(uint n);          // Create graph with n vertices, no adjacency
    graph(const graph& G);  // Copy constructor
// Destructor
    ~graph(void);           // Destructor. Free memory from array adj
// Overloads
    graph operator=(const graph& G);
      // Assignment operator
    void reset();           // Reset all vertices to default state
    void percolate(double p, uint seed=314);
      // Probabilistically delete edges.
    void bfs(uint start, uint dir, uint id=0);
    void bfs(std::queue<vertex*>* Q, uint dir, uint id=0);
      // Breadth first search routines starting with a single vertex or set of
      // queued vertices
// Access methods
    void print(void) const; // Print summary of graph to cout
};

#endif
