#include <string>

#include "Interface.hpp"

////////////////////////////////////////////////////////////////////////
//                        CORE
////////////////////////////////////////////////////////////////////////
/*
  cell, action, and grid are level 0 objects.
  they are dumb and blind, only providing basic functionality
  any controls, error checking, info hiding is in higher levels
*/

class Mind;

/* Because an Action can be created and never be used,
   (in the case of a cell who chooses an action it does not have
   enough energy to perform) an Action's constructor needs to not modify
   anything outside of the Action itself,and must be able to clean itself up with the destructor */
struct Action{

  int cost;
  std::string name;

  Action(int c, std::string n): cost(c), name(n)
  {}

  virtual void act(int x, int y, Grid<Cell*>* g)=0;

  virtual void cleanup(){}
  
};

struct Cell{
  Mind *mind;
  int energy, team;

  Cell(Mind *m, int ie, int t): mind(m), energy(ie), team(t)
  {}

};

////////////////////////////////////////////////////////////////////////
//                        ACTIONS
////////////////////////////////////////////////////////////////////////

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

struct Share: public Action {
  int direction;
  Share(int dir):Action(0, "Share"), direction(dir)
  {}

  void act(int x, int y, Grid<Cell*>* grid){
    int be;
    Cell* self = grid->get(x,y);
    self->energy--;
    Cell* nbr = grid->get(x, y, direction);
    nbr->energy++;
  }
  
};

struct Nop: public Action {

  Nop():Action(0, "Nop")
  {}

  void act(int x, int y, Grid<Cell*>* g){}
  
};

#define UP 0
#define DOWN 2
#define LEFT 1
#define RIGHT 3

class View{

  /* Interface for minds to interact with cells.
     For efficiency's sake, there is a single View created for an entire game, that
     has a transition method for switching between cells. */

  int x,y;
  Cell* cell;
  Grid<Cell*>* grid;
  Action* action;
  
  int Bvalid(int dir){
    return grid->inBounds(x, y, dir);
  }

  Cell* getNeighbor(int dir){
    grid->get(x,y, dir);
  }

  //assumes a valid action (action must be verified before calling this)
  void doAction(){
    if (cell->energy < action->cost){
      action->cleanup();
      throw Bone("Can't afford action!");
    }
    cell->energy -= action->cost;
    action->act(x,y,grid);      
  }

public:

  //assuming cell is non null
  View(Grid<Cell*>* g, int xx, int yy):
    cell(g->get(xx, yy)), x(xx), y(yy), grid(g)
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

};

struct Mind{

  virtual Action* decide(View v)=0;
  
};

struct Simp: public Mind{
  Action* decide(View v){
    //POL("energy: " << v.getEnergy());
    if (v.getEnergy() < 102)
      return new Feed();
    else{
      for (int i=0; i<4; i++){
	if(v.valid(i) && !v.look(i)){
	  return new Divide(i, new Simp());
	}
      } 
      return new Feed();
    }
  }
};



class CircInt {

  int n, base;
  
public:

  CircInt(int i, int b):
    n(i), base(b){}

  CircInt(int b):
    n(0), base(b){}

  int inc(int x){
    return (n = (n+x)%base);
  }

  int dec(int x){
    return (n = abs(n-x)%base);
  }
  
};

struct SpydrVine: public Mind{
  int state, dir,spawnDir, count;

  SpydrVine(int d): dir(d), state(0), spawnDir(-1), count(0)
  {}

  //decide which direction to divide to, -1 for none
  int getDir(View *v){

    int d = dir;
    CircInt di(dir, 4);
    if (v->valid(d) && !v->look(d)) return d;
    d = di.inc(2);
    if (v->valid(d) && !v->look(d)) return d;
    d = di.inc(1);
    if (v->valid(d) && !v->look(d)) return d;
    d = di.inc(2);
    if (v->valid(d) && !v->look(d)) return d;
    return -1;
  }
  
  Action* decide(View v){
    count --;
    if (spawnDir == -1){
      if (v.getEnergy() > 102){
	int gd = getDir(&v);
	if (gd != -1){
	  spawnDir = gd;
	  count = 200;	  
	  return new Divide(gd, new SpydrVine(gd));
	    }
      }
      return new Feed();
    }else {
      if (count > 1 && v.getEnergy() > 10 && v.look(spawnDir)){
	return new Share(spawnDir);
      }
      if (v.getEnergy() > 102){
	int gd = getDir(&v);
	if (gd != -1){
	  return new Divide(gd, new SpydrVine(gd));
	    }
      }
      return new Feed();
    }
  }
};

struct Giver: public Mind{
  Action* decide(View v){
    if (v.getEnergy() < 10)
      return new Feed();
    else if (v.getEnergy() < 102){
      for (int i=3; i<4; i++){
	if(v.valid(i) && v.look(i)){
	  return new Share(i);
	}
      }
    }
    else{
      for (int i=0; i<4; i++){
	if(v.valid(i) && !v.look(i)){
	  return new Divide(i, new Giver());
	}
      }
      for (int i=0; i<4; i++){
	if(v.valid(i)){
	  return new Share(i);
	}
      }
    }
    return new Feed();
  }
};



void feed(Action *a, View *vw){
  Feed* f = dynamic_cast<Feed*>(a);
  if(f != NULL) ;
  else throw Bone("Mislabeled Action! (Not a Feed)");
}

void divide(Action *a, View *vw){

  Divide* d = dynamic_cast<Divide*>(a);
  if(d != NULL){
    if (! vw->valid(d->direction))
      throw Bone("Dividing out of bounds!");
    
    Cell* nbr = vw->look(d->direction);
    if (nbr) throw Bone("Dividing into occupied space");
  }
  else throw Bone("Mislabeled Action! (Not a Divide)");
}

void share(Action *a, View *vw){

  Share* s = dynamic_cast<Share*>(a);
  if(s != NULL){
    if (! vw->valid(s->direction))
      throw Bone("Sharing out of bounds!");
    
    Cell* nbr = vw->look(s->direction);
    if (!nbr) throw Bone("Sharing with unoccupied space");
  }
  else throw Bone("Mislabeled Action! (Not a Share)");
}

void nop(Action *a, View *vw){}

#include <map>

typedef void (*CheckPtr)(Action*, View*); // function pointer type
typedef std::map<std::string, CheckPtr> CheckPtr_map;


class ActionCheck {

  CheckPtr_map map;

public:
  ActionCheck(){
    map.emplace("Feed", &feed);
    map.emplace("Divide", &divide);
    map.emplace("Share", &share);
    map.emplace("Nop", &nop);
  }

  void approval(Action *a, View *vw){
    if(map[a->name])
      map[a->name](a, vw);
    else throw Bone("Action not found in map: "+a->name);
  }
  
};
ActionCheck actionCheck;
  
  CellGame::CellGame(int s, int t):
    turns(t), size(s), grid(s, s), c(0)
  {
    //randomized placement of all participating minds
    //grid.set(new Cell(new Simp(), 1, 1), 0, 0);

        grid.set(new Cell(new SpydrVine(0), 1, 5), 0, 0);

	grid.set(new Cell(new Simp(), 1, 1), 20, 20);
	grid.set(new Cell(new Giver(), 1, 2), 40, 40);
	grid.set(new Cell(new Simp(), 1, 1), 15, 15);

    grid.set(new Cell(new Giver(), 1, 2), 25, 25);

    //grid.set(new Cell(new Giver(), 1, 4), 0, 10);
  }

  Grid<int>* CellGame::turn(){

    if (c++ == turns)
      return new Grid<int>(size, size); //blank board signifies end of game
    
    for (int y=0; y<grid.height; y++){
      for (int x=0; x<grid.width; x++){
	Cell* c = grid.get(x,y);
	if (c){
	  try {
	    View vw = View(&grid, x, y);
	    Action *a = c->mind->decide(vw);

	    actionCheck.approval(a, &vw);
	    a->act(x,y,&grid);
	  }
	  catch (Bone b){
	    POL(b.what());
	  }}}}
    return convert();
  }


  //could be converted into a template fn inside Grid {Cell* -> int}
//converts board into consumabl for opengl
  Grid<int>* CellGame::convert(){
    Grid<int>* pg = new Grid<int>(size, size);

    Cell* c;
    for (int y=0; y<grid.height; y++){
      for (int x=0; x<grid.width; x++){
	pg->set((!(c=grid.get(x,y)))?0:c->team, x, y);
      }}
    
    return pg;
  }

//careful when making sever, will need to remove pointers and stuff
// void Cell::f_sever(Cell* neighbor){
//   //make sure neighbor is actually connected to this cell
//   //if so
//   disconnect(neighbor);
// }
// sever = new Action(5, f_sever);


