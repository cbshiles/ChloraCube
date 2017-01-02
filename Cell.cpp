#include "Cell.hpp"

Cell::Cell():energy(1){
  
}

Cell::f_feed(){
  energy++;
}
feed = new Action(0, f_feed);

Cell::f_divide(){
  int be = energy/2; //baby energy
  connect(new Cell(be));
  energy -= be;
}
divide = new Action(100, f_divide);
  
Cell::f_share(Cell *receiver, int amt){
  //cant take energy, or give over 5 energy at a time
  if (amt < 0) amt=0;
  else if (amt > 5) amt=5;

  //can't give more energy than you have
  if (energy < amt) amt=energy;

  //the transaction
  energy -= amt;
  receiver->energy += amt;
}
share = new Action(1, f_share);

void Cell::f_sever(Cell* neighbor){
  //make sure neighbor is actually connected to this cell
  //if so
  disconnect(neighbor);
}
sever = new Action(5, f_sever);

int Cell::energyLevel(){return energy;}

void SimpCell::action(){
  if (energyLevel() > 104){
    divide();
  }
}
