#include <string>

#include "Interface.hpp"

class Mind;

/*
  cell, action, and grid are level 0 objects.
  they are dumb and blind, only providing basic functionality
  any controls, error checking, info hiding is in higher levels
*/

struct Cell{
  Mind *mind;
  int energy, team;

  Cell(Mind *m, int ie, int t): mind(m), energy(ie), team(t)
  {}

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
    grid->set(new Cell(mind, be, self->team), x, y, direction);
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

struct Simp: public Mind{
  void decide(View v){
    POL("energy: " << v.getEnergy());
    if (v.getEnergy() < 102)
      v.feed();
    else{
      for (int i=0; i<4; i++){
	if(v.valid(i) && !v.look(i)){
	  v.divide(i, new Simp());
	  return;
	}
      }
      v.feed();
    }
  }
};

  
  CellGame::CellGame(int s, int t):
    turns(t), size(s), grid(s, s), c(0)
  {
    //randomized placement of all participating minds
    grid.set(new Cell(new Simp(), 1, 1), 0, 0);

    grid.set(new Cell(new Simp(), 1, 2), 10, 10);

        grid.set(new Cell(new Simp(), 1, 3), 17, 17);
  }

  Grid<int>* CellGame::turn(){

    if (c++ == turns)
      return new Grid<int>(size, size); //blank board signifies end of game
    
    for (int y=0; y<grid.height; y++){
      for (int x=0; x<grid.width; x++){
	Cell* c = grid.get(x,y);
	try {
	  if (c){
	    c->mind->decide(View(&grid, x, y));
	  }
	} catch (Bone b){
	  POL(b.what());
	}}}
    return convert();
  }


  //could be converted into a template fn inside Grid {Cell* -> int}
  Grid<int>* CellGame::convert(){
    Grid<int>* pg = new Grid<int>(size, size);

    Cell* c;
    for (int y=0; y<grid.height; y++){
      for (int x=0; x<grid.width; x++){
	pg->set((!(c=grid.get(x,y)))?0:c->team, x, y);
      }}
    
    return pg;
  }

/*

class Share: public Action {
private:
  Cell* receiver;
  int max, amt; //amount

public:
  Share(Cell* r, int a): Action(1), receiver(r), max(5){
    //cant take energy, or give over max energy at a time
    if (a < 0) amt=0;
    else if (a > max) amt=max;
    else amt = a;
  }

  void act(Cell* target){
    //can't give more energy than you have
    if (target->energy < amt) amt=target->energy;

    //the transaction
    target->energy -= amt;
    receiver->energy += amt;
  }
};
*/

//careful when making sever, will need to remove pointers and stuff
// void Cell::f_sever(Cell* neighbor){
//   //make sure neighbor is actually connected to this cell
//   //if so
//   disconnect(neighbor);
// }
// sever = new Action(5, f_sever);


