struct Action {
  int cost;
  void (*doo)();
  Action(int c, void(*d)()):cost(c), doo(d){}
};

class Cell {
private:
  int energy;
public:
  Cell();
  virtual Action* act();
  int energyLevel();

  int score();
  //Core set of actions
  void divide();
};

class SimpCell: public Cell {

public:
  void action();  
};
