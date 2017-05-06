#include "Gui.hpp"

Screen *s;

CellGame *game;

void mouseInput(int button, int state, int x, int y){
  ; //oh, nothin
}

void drawScene(){
   s -> draw(game->turn());
}

void resize(int x, int y){
  s -> dims(x,y);
}

int main(int argc, char* argv[]){
  int sz = 25;
  int turns = 3000;
  s = new Screen("Chlora", sz);
  game = new CellGame(sz, turns);
  glutMainLoop();
}
