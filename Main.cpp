#include <iostream>

#include "Cell.hpp"
#include "Life.hpp"

int main(int argc, char* argv[]){

  SimpCell test;
  play(test);
  std::cout << test.score();

  std::cout << "Hello, My World!\n";
}
