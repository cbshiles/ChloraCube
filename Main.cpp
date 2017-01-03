#include <iostream>

#include "Cell.hpp"

#define ROUNDS 1000

void play(Cell *cell){
  int i;
  for (i=0; i<ROUNDS; i++){
    cell->move();
  }
}

int main(int argc, char* argv[]){
  SimpCell test;
  play(&test);
  std::cout << "Hello, My World!\n";
}
