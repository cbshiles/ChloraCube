//#include <always.h>

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

template <class T>
struct Grid{

  int width, height;
  std::vector<T*> data;
  
  Grid(int x, int y):
    width(x), height(y)
  { 
    for (int i=0; i<x*y; i++)
      data.push_back(0);
  }

  inline char inBounds(int x, int y){
    return x>0 && x<width && y>0 && y<height;
  }

  inline T* get(int x, int y){return data[FLAT(x,y)];}

  inline char empty(int x, int y){return !(*get(x,y));}
  
  void bohPrint(){
    for (int j=0; j<height; j++){
      for (int i=0; i<width; i++)
	PO( &data[i+j*width] << " ");
      POL("");
    }}
};

//# move into another file

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
  }
};

int main(){
  Grid<Cell> dish(10, 10);
  dish.bohPrint();
  lookup(dish.get(5,5));
  return 0;
}
