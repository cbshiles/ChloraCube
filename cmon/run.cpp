#include "start.hpp"

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


#define TURNS 1000

int main(){
  Grid<Cell*> grid(10, 10);

  grid.set(new Cell(new Simp(), 1), 0, 0);

  for (int i=0; i<TURNS; i++){
    for (int y=0; y<grid.height; y++){
      for (int x=0; x<grid.width; x++){
	Cell* c = grid.get(x,y);
	try {
	  if (c){
	    POL(x<<y<<"HERE !" << c->mind);
	    c->mind->decide(View(&grid, x, y));
	    POL("THERE !");
	  }
	} catch (Bone b){
	  POL(b.what());
	}
      }
    }
  }
}
