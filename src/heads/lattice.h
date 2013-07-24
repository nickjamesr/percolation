// lattice.h
// Header file for lattice class

#ifndef lattice_h
#define lattice_h

#include <iostream>
#include <cstdlib>
#include <vector>

#include "graph.h"

class lattice_t{
  private:
    class coord{
      public:
        coord(){h=i=j=k=0;};
        coord(int, int, int, int);
        coord(const coord&);
        int h;
        int i;
        int j;
        int k;
    };
  protected:
  public:
    // Attributes
    uint size;
    std::vector<coord>* adjacency;
    std::string label;
    // Constructors
    lattice_t(void);
    lattice_t(const lattice_t&);
    lattice_t(uint, std::string);
    // Destructors
    ~lattice_t(void);
    // Overloads
    lattice_t operator=(const lattice_t);
    // Access methods
    void add(uint, int, int, int, int);
    void print(void);
};

class lattice: public graph{
  private:
    uint dimx;
    uint dimy;
    uint dimz;
    lattice_t type;
  protected:
  public:
    // Constructors
    lattice(void);
    lattice(const lattice&);
    lattice(lattice_t,uint,uint,uint);
    // Destructor
    ~lattice(void);
    // Overloads
    lattice operator=(const lattice);
    // Access methods
    void print(void);
};

namespace lattices{
  lattice_t raussendorf(void);
  lattice_t cubic(void);
  lattice_t diamond(void);
};

#endif
