#define P(dest, s) (dest << s)
#define PO(s) (P(std::cout, s))

#define PL(dest, s) (dest << s << std::endl)
#define POL(s) (PL(std::cout, s))
#define PEL(s) (PL(std::cerr, s))
#define POS(s) (POL(#s << " " << s))

#include <vector>
#include <string>
#include <iostream>

#define FLAT(x, y) (x+y*width)
#define FOLD(z) (z%width, z/width)

template <struct T>
struct Grid{

  std::vector<T*> data;

    Grid(int x, int y)
  { 
    for (int i=0; i<x*y; i++)
      data.push_back(0);
  }

  int inBounds(int x, int y){
    int z = FLAT(x,y);
    return z>=0 && z<data.length;
  }

   T* get(int x, int y){return data[FLAT(x,y)];}
};

struct Cell {
  int energy;
  Cell(int ie): energy(ie){}
};
