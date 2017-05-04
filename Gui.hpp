#include "MyGL.hpp"
#include <iostream>
#include "Interface.hpp"
/*
  This is a page we might just have to copy each time we use a new glut loop,
  unlike MyGL which should be moved to a centralized location */


void mouseInput(int button, int state, int x, int y);

void drawScene();

void resize(int x, int y);

#define AS 20
#define AX 30
#define AY 30
#define GB 5
/* Ugly bolierplate class, but it isn't easy to avoid. */
class Screen {

public:
  int xdim,ydim,size;
  std::vector<Color*> colorArray;
  

  Screen(char* name, int s):
    colorArray({white,orange}), size(s) 
  {

    xdim = AX*(AS+GB)+GB;
    ydim = AY*(AS+GB)+GB;

    // Initialize GLUT.
    char fakeParam[] = "FuckTrump.cpp";
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
  }

  
  void dims(int x, int y){
    //# doesnt reallly resize..
    xdim = x, ydim = y;
  }

  void draw(Grid<int>* colors){
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)

    black->setBackground();
    POL("BEFORE LOOP");
    int q=0; int x=0; int y=0;
    for (double i=-1.0+GB/(AS*1.0*AX); x<size; x++, i+=(GB+AS+0.0)/xdim){
      for (double j=-1.0+GB/(AS*1.0*AX); y<size; y++, j+=(GB+AS+0.0)/xdim){
	POL("."<<x<<" "<<y);

	colorArray[colors->get(x,y)]->set();
	  POL("~");

	glBegin(GL_POLYGON);
	glVertex3f(i, j, 0);
	glVertex3f(i, j+(AS*1.0/xdim), 0);
	glVertex3f(i+(AS*1.0/ydim), j+(AS*1.0/xdim), 0);
	glVertex3f(i+(AS*1.0/ydim), j, 0);
	glEnd();
      }
    }
    POL("AFTER LOOP");
      glFlush();
    glutPostRedisplay();
  }
};
