#include "MyGL.hpp"

void mouseInput(int button, int state, int x, int y);

void drawScene();

void resize(int x, int y);

class Screen {
  void init() {;}

public:
  int xdim,ydim;

  Screen(int x, int y, char* name):xdim(x),ydim(y){
    // Initialize GLUT.
    char fakeParam[] = "GameDriver.cpp";
    char *fakeargv[] = { fakeParam, NULL };
    int fakeargc = 1;
    glutInit(&fakeargc, fakeargv);
 
    // Set display mode as single-buffered and RGB color.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 
   
    // Set OpenGL window size.
    glutInitWindowSize(xdim,ydim);

    // Set position of OpenGL window upper-left corner.
    glutInitWindowPosition(100, 100); 
   
    // Create OpenGL window with title.
    glutCreateWindow(name);
   
    // Initialize.
    GL_init(); 

    glutMouseFunc(mouseInput);
    
    // Register display routine.
    glutDisplayFunc(drawScene); 
   
    // Register reshape routine.
    glutReshapeFunc(resize);

    init();
  }

  
  void dims(int x, int y){
    //# doesnt reallly resize..
    xdim = x, ydim = y;
  }

  void draw(){;}
};
