#include <vector>

template <class T>
struct Grid{

  int width, height;
  std::vector<T> data;//the actual grid
  
  Grid(int x, int y):
    width(x), height(y)
  { 
    for (int i=0; i<x*y; i++)
      data.push_back(0);
  }

  char inBounds(int x, int y){
    return x>=0 && x<width && y>=0 && y<height;
  }

  inline char inBounds(int x, int y, int dir){
    locate(x,y,dir);
    return inBounds(x0, y0);
  }

  inline void set(T thing, int x, int y){data[x+y*width] = thing;}

  inline void set(T thing, int x, int y, int dir){
    locate(x,y,dir);
    set(thing, x0, y0);
  }
  
  inline T get(int x, int y){return data[x+y*width];}

  inline T get(int x, int y, int dir){
    locate(x,y,dir);
    return get(x0, y0);
  }
  
  void bohPrint(){
    for (int j=0; j<height; j++){
      for (int i=0; i<width; i++)
	PO( &data[i+j*width] << " ");
      POL("");
    }}

  int x0, y0;
  
  int locate(int x, int y, int dir){
    if (dir%2){
      x0 = x+dir-2;
      y0 = y;
    } else {
      x0 = x;
      y0 = y+dir-1;
    }
  }

};
