#define P(dest, s) (dest << s)
#define PO(s) (P(std::cout, s))
#define PL(dest, s) (dest << s << std::endl)
#define POL(s) (PL(std::cout, s))
#define PEL(s) (PL(std::cerr, s))
#define POS(s) (POL(#s << " " << s))

class Cell;

class Action {
public:
  Action(int c): cost(c){}
  int cost;
  virtual void act(Cell* target)=0;
  virtual void cleanup(){} //called if requirements to call act not met
};

//need to have destructor (or make sure default one is good enough)
struct Cell {
  Cell(int ie);

  int energy;
  Cell* connects[4]; //quad board for now
  void act(Action* a);
};
