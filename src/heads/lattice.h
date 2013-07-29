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
    int geth(int h, int n) const{ return adjacency[h][n].h; };
    int geti(int h, int n) const{ return adjacency[h][n].i; };
    int getj(int h, int n) const{ return adjacency[h][n].j; };
    int getk(int h, int n) const{ return adjacency[h][n].k; };
    void print(void);
};

class lattice: public graph{
  private:
    uint dimx;
    uint dimy;
    uint dimz;
    lattice_t type;
    uint fromCoord(int, int, int, int);
    class iterator{
      private:
        uint size[4];
        uint n;
      public:
        uint coord[4];
        iterator();
        iterator(uint, uint, uint, uint);
        uint index(void) const {return n;};
        void operator++(int);
        uint operator[](uint) const;
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
    lattice operator=(const lattice);
    // Access methods
    vertex* traverse(void);
    void trace(vertex*);
    void print(void);
};

namespace lattices{
  lattice_t raussendorf(void);
  lattice_t cubic(void);
  lattice_t diamond(void);
};

#endif
