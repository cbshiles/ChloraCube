#include "Gui.hpp"

Screen *s;

void mouseInput(int button, int state, int x, int y){
  ; //oh, nothin
}

void drawScene(){
   s -> draw();
}

void resize(int x, int y){
  s -> dims(x,y);
}

int main(int argc, char* argv[]){
  s = new Screen(500, 500, "Chlora");
}
