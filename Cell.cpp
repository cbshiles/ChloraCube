#include "Cell.hpp"

Cell::Cell():energy(1){}

void Cell::link(Cell* c){
  connects.push_back(c);
  c->connects.push_back(this);
}

void Cell::move(){
  Action *a = decide();
  energy -= a->cost;

  if (energy <= 0){
    a->cleanup();
    //remove this cell
  } else {
    a->act(this);
  }
  //delete *a
}

int Cell::energyLevel(){return energy;}

class Feed : public Action {
private:
  int rate;
public:
  Feed(): Action(0), rate(1){}

  void act(Cell* target){
    target->energy += rate;
  }
};

class Divide : public Action {
private:
  Cell *baby;  
public:
  Divide(Cell *b): Action(100), baby(b){}

  void act(Cell* target){
    int be = target->energy/2; //baby energy
    target->link(baby);
    target->energy -= be;
    baby->energy = be;
  }

  void cleanup(){
    //delete baby
    //memory leak until
  }
};

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

//careful when making sever, will need to remove pointers and stuff
// void Cell::f_sever(Cell* neighbor){
//   //make sure neighbor is actually connected to this cell
//   //if so
//   disconnect(neighbor);
// }
// sever = new Action(5, f_sever);

Action* SimpCell::decide(){
  if (energyLevel() > 104){
    return new Divide(new SimpCell()); 
  }
  else return new Feed();
}
