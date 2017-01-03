#include <vector>

class Cell;

class Action {
public:
  Action(int c): cost(c){}
  int cost;
  virtual void act(Cell* target)=0;
  virtual void cleanup(){} //called if requirements to call act not met
};

//need to have destructor (or make sure default one is good enough)
class Cell {
  friend class Feed;
  friend class Divide;
  friend class Share;
private:
  int energy;
  std::vector<Cell*> connects;
  void link(Cell* c);
  virtual Action* decide()=0;
  
public:
  Cell();
  Cell(int ie);
  void move();
  int energyLevel();
};

class SimpCell: public Cell {
public:
  Action* decide();  
};
