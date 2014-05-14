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
        coord(const coord& C){h=C.h; i=C.i; j=C.j; k=C.j;};
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
    std::string label;
    // Constructors
    lattice_t(void);
    lattice_t(const lattice_t&);
    lattice_t(uint, std::string);
    // Destructors
    ~lattice_t(void);
    // Overloads
    lattice_t operator=(const lattice_t&);
    // Access methods
    void add(uint, int, int, int, int);
    void print(void);
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
    // Constructors
    lattice(void);
    lattice(const lattice&);
    lattice(lattice_t,uint,uint,uint);
    // Destructor
    ~lattice(void);
    // Overloads
    lattice operator=(const lattice&);
    // BFS-type stuff
    void traverse();
    std::vector<uint> findCrossings();
    // Access methods
    std::string label() const {return type.label;};
    void print(void);
};

namespace lattices{
  lattice_t raussendorf(void);
  lattice_t cubic(void);
  lattice_t diamond(void);
};

#endif
