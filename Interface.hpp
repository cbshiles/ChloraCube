#include <iostream>

#define P(dest, s) (dest << s)
#define PO(s) (P(std::cout, s))

#define PL(dest, s) (dest << s << std::endl)
#define POL(s) (PL(std::cout, s))
#define PEL(s) (PL(std::cerr, s))
#define POS(s) (POL(#s << " " << s))

struct Bone: public std::exception
{
  const char* msg;

  Bone(const char* gsm): msg(gsm)
  {}
  
  virtual const char* what() const throw()
  {
    return msg;
  }
};

#include "Grid.hpp"

struct Cell;

struct CellGame{

  Grid<Cell*> grid;
  int size, turns, c;
  
  CellGame(int s, int t);
  
  Grid<int>* turn();
private:
    Grid<int>* convert();
};
