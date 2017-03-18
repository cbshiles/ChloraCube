#define P(dest, s) (dest << s)
#define PO(s) (P(std::cout, s))

#define PL(dest, s) (dest << s << std::endl)
#define POL(s) (PL(std::cout, s))
#define PEL(s) (PL(std::cerr, s))
#define POS(s) (POL(#s << " " << s))

#include <vector>
#include <string>
#include <iostream>

struct Grid<T> {

  int width, height;
  std::vector<std::vector<*T > > data;//the actual grid
  
  Grid(int x, int y):
    width(x), height(y)
  { 
    for (int j=0; j<y; j++){
      data.push_back(std::vector<*T >());
      for (int i=0; i<x; i++)
	data[j].push_back(0);
    }
  }

  char inBounds(int x, int y){
    return x>0 && x<width && y>0 && y<height;
  }

  T* get(int x, int y){return data[y][x];}

  char empty(int x, int y){return ! data[y][x];}
  
  void bohPrint(){
    for (int j=0; j<height; j++){
      for (int i=0; i<width; i++)
	PO(data[j][i] << " ");
      POL("");
    }}

  /* NOTE: not guaranteed to work.
     if youre on the top row and try to go up,
     its YOUR ass */

  /* dir code
     up - 0
     right - 1
     down - 2
     left - 3
  */
  T* move(T* start, int dir){
    return start +=
      (dir%2)?
      dir-2:
      (dir-1)*width;
  }
};

//# move into another file

/* Get grid, and then uncomment these

struct Action;

struct Cell {
  Cell(int ie): energy(ie){}
  int energy;
};

struct Action {
  Action(int c): cost(c){}
  int cost;
  virtual void act(Cell* target)=0;
};
/* Because an Action can be created and never be used,
(in the case of a cell who chooses an action it does not have
enough energy to perform) an Action's constructor needs to not modify
anything outside of the Action itself */

/* cog in the machine

struct Feed : public Action {
  Feed(): Action(0), rate(1){}

  int rate;
  void act(Cell* target){
    target->energy += rate;
  }
};

struct Divide: public Action {
  Divide(int dir): Action(100), direction(dir){}

  int direction;
  void act(Cell* target){
    //mmm.. pointer arithmetic!
    /*
      get the 2d vector goin
      start printing out the pointers themselves
      in a grid pattern
     */
//  }
// };
