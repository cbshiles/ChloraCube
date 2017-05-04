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
  s = new Screen("Chlora", 15);
  game = new CellGame(15, 1000);
  glutMainLoop();
}
