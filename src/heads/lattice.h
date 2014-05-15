// lattice.h
// Header file for lattice class

#ifndef lattice_h
#define lattice_h

#include <iostream>
#include <cstdlib>
#include <vector>

#include "graph.h"

class lattice_t{
/* lattice type class.
 * Describes a unit cell, required to build lattices.
 */
  private:
    class coord{
    /* coordinate class.
     * Just a 4-vector really. Helps out with building lattice
     */
      public:
        coord(){h=i=j=k=0;};        // Empty constructor. Initialise to zero.
        coord(int w, int x, int y, int z){h=w; i=x; j=y; k=z;};
                                    // Constructor with specified 4-vector.
        coord(const coord& C){h=C.h; i=C.i; j=C.j; k=C.k;};
                                    // Copy constructor.
        int h;  // Internal to unit cell
        int i;  // In x direction
        int j;  // In y direction
        int k;  // In z direction
    };
  protected:
  public:
    // Attributes
    uint size;            // Number of vertices in unit cell
    std::vector<coord>* adjacency;
                          // Connections out of unit cell
    std::vector<uint> startx, starty, startz, endx, endy, endz;
                          // Starting (finishing) vertices for crossing clusters
    std::string label;    // Name of the unit cell
    lattice_t(void);      // Empty constructor
    lattice_t(const lattice_t& D);
                          // Copy constructor
    lattice_t(uint n, std::string s);
                          // New constructor
    ~lattice_t(void);     // Destructor
    lattice_t operator=(const lattice_t& D);
                          // Assignment operator
    // Access methods
    void add(uint start, int h, int i, int j, int k);
                          // Add new connection to unit cell
    void print(void);     // Print summary of unit cell to cout
};

class lattice: public graph{
/* Lattice class. Derived from graph.
 * Allows construction and handling of graphs with information about unit cell,
 * height, width and depth (in 3d)
 */
  private:
    // Dimensions of graph (number of unit cells in 
    uint dimx;       // x
    uint dimy;       // y
    uint dimz;       // z directions
    lattice_t type;  // Unit cell
    uint fromCoord(int h, int i, int j, int k)
      {return h+type.size*(i+dimx*(j+dimy*k));};
                     // Convert 4D coordinate to 1D index
    class iterator{
    /* Iterate through lattices without having to write 4 nested for loops.
     * Never written one before, so I expect it's a bit dodge.
     * Should probably be a public member too, so it can be used outside the
     * class.
     * Anyway, it works for now
     */
      private:
        uint size[4];     // Maxima in 4 directions
        uint n;           // Current position as 1D index
      public:
        uint coord[4];    // Current position as 4D coordinate
        iterator();       // Empty constructor
        iterator(uint dimw, uint dimx, uint dimy, uint dimz);
                          // Construct with max size. Initialize to zero.
        uint index(void) const {return n;};
                          // Return the current position as 1D index
        uint operator[](uint) const;
                          // Return the i-th element of the current 4-position
        // The usual overloads for incrementing and comparison.
        void operator++(int);
        bool operator<(uint) const;
        bool operator>(uint) const;
        bool operator<=(uint) const;
        bool operator>=(uint) const;
    };
  protected:
  public:
    lattice(void);                // Empty constructor
    lattice(const lattice& lat);  // Copy constructor
    lattice(lattice_t D, uint L, uint M, uint N);
                                  // Construct a LxMxN lattice with unit cell D
    ~lattice(void);               // Destructor
    lattice operator=(const lattice&);
                                  // Assignment operator
    // BFS-type stuff
    void traverse();              // Perform bfs in all 6 directions
    std::vector<uint> findCrossings();
                                  // Find the smallest crossing clusters
    // Access methods
    std::string label() const {return type.label;};
                                  // Return the text label indicating the type
                                  // of lattice
    void print(void);             // Print summary of lattice to cout
};

namespace lattices{
  lattice_t raussendorf(void);
  lattice_t cubic(void);
  lattice_t diamond(void);
  lattice_t diamond_grid(void);
};

#endif
