#include <vector>
#include <string>

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


class Mind;

/*
  cell, action, and grid are level 0 objects.
  they are dumb and blind, only providing basic functionality
  any controls, error checking, info hiding is in higher levels
*/

struct Cell{
  Mind *mind;
  int energy;

  Cell(Mind *m, int ie): mind(m), energy(ie)
  {}

};

template <class T>
struct Grid{

  int width, height;
  std::vector<T> data;//the actual grid
  
  Grid(int x, int y):
    width(x), height(y)
  { 
    for (int i=0; i<x*y; i++)
      data.push_back(0);
  }

  char inBounds(int x, int y){
    return x>=0 && x<width && y>=0 && y<height;
  }

  inline char inBounds(int x, int y, int dir){
    locate(x,y,dir);
    return inBounds(x0, y0);
  }

  inline void set(T thing, int x, int y){data[x+y*width] = thing;}

  inline void set(T thing, int x, int y, int dir){
    locate(x,y,dir);
    set(thing, x0, y0);
  }
  
  inline T get(int x, int y){return data[x+y*width];}

  inline T get(int x, int y, int dir){
    locate(x,y,dir);
    return get(x0, y0);
  }
  
  void bohPrint(){
    for (int j=0; j<height; j++){
      for (int i=0; i<width; i++)
	PO( &data[i+j*width] << " ");
      POL("");
    }}

  int x0, y0;
  
  int locate(int x, int y, int dir){
    if (dir%2){
      x0 = x+dir-2;
      y0 = y;
    } else {
      x0 = x;
      y0 = y+dir-1;
    }
  }

};

/* Because an Action can be created and never be used,
   (in the case of a cell who chooses an action it does not have
   enough energy to perform) an Action's constructor needs to not modify
   anything outside of the Action itself */
struct Action{

  int cost;
  std::string name;

  Action(int c, std::string n): cost(c), name(n)
  {}

  virtual void act(int x, int y, Grid<Cell*>* g)=0;

  virtual void cleanup(){}
  
};

struct Feed : public Action {
  Feed(): Action(0, "Feed"), rate(1)
  {}

  int rate;
  void act(int x, int y, Grid<Cell*>* g){
    g->get(x,y)->energy += rate;
  }
  
};

struct Divide: public Action {
  Mind *mind;
  
  Divide(int dir, Mind* m): Action(100, "Divide"), direction(dir), mind(m)
  {}

  int direction;

  void act(int x, int y, Grid<Cell*>* grid){
    int be;
    Cell* self = grid->get(x,y);
    self->energy = (be = self->energy/2);
    grid->set(new Cell(mind, be), x, y, direction);
  }

  void cleanup(){
    //delete mind; //!problem!
  }
};

#define UP 0
#define DOWN 2
#define LEFT 1
#define RIGHT 3

class View{

  /* Interface for minds to interact with cells.
     For efficiency's sake, there is a single View created for an entire game, that
     has a transition method for switching between cells. */

  char spent;
  int x,y;
  Cell* cell;
  Grid<Cell*>* grid;
  Action* action;
  
  int Bvalid(int dir){
    POL(x<< " " << y<< " " << dir);
    grid->inBounds(x, y, dir);
  }

  Cell* getNeighbor(int dir){
    grid->get(x,y, dir);
  }

  void doAction(){

    POL(x << " " << y<< " "<< action->name);
    
    if (spent++) throw Bone("Already moved!");

    cell->energy -= action->cost;
    
    if (cell->energy >= 0){
      action->act(x,y,grid);      
    } else {
      action->cleanup();
      delete cell;
    }

    POL("End of: "<<x << " " << y<< " "<< action->name);
  }

public:

  //assuming cell is non null
  View(Grid<Cell*>* g, int xx, int yy):
    cell(g->get(xx, yy)), x(xx), y(yy), grid(g),
    spent(0)
  { }

  int valid(int dir){
    if (dir < 0 || dir > 3)
      throw Bone("Invalid direction code");
    return Bvalid(dir);
  }

  Cell* look(int dir){
    if (valid(dir))
      getNeighbor(dir);
    else throw Bone("Invalid look request.");
  }
  
  int getEnergy(){
    return cell->energy;
  }

  void feed(){
    action = new Feed();
    doAction();
  }

  void divide(int dir, Mind* m){

    if (! grid->inBounds(x,y,dir))
      throw Bone("Dividing out of bounds!");
    
    Cell* nbr = grid->get(x,y,dir);
    if (nbr) throw Bone("Dividing into occupied space");

    action = new Divide(dir, m);
    doAction();
  }

};

struct Mind{

  virtual void decide(View v)=0;
  
};



/*
  #include <iostream>
  #include <exception>
  using namespace std;

  class myexception: public exception
  {
  virtual const char* what() const throw()
  {
  return "My exception happened";
  }
  } myex;

  int main () {
  try
  {
  throw myex;
  }
  catch (exception& e)
  {
  cout << e.what() << '\n';
  }
  return 0;
  } */
/* Interface for minds to interact with cells.
   For efficiency's sake, there is a single View created for an entire game, that
   has a transition method for switching between cells. */

