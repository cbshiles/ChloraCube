#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <stdio.h>

struct Color {
    double r, g, b;

    Color(Color& c):
	r(c.r), g(c.g), b(c.b){};

    Color(double rr, double gg, double bb) :
	r(rr), g(gg), b(bb){};

    Color(int rr, int gg, int bb) :
	r(rr/255.0), g(gg/255.0), b(bb/255.0) {}
    
    void set() {glColor3f(r, g, b);}

    void setBackground() {glClearColor(r,g,b,0);}

    void print() {printf("%f %f %f\n",r,g,b);}
};

Color* white = new Color(255,255,255);
Color* blue = new Color(17,31,92);
Color* red = new Color(227,51,38);
Color* orange = new Color(255,150,0);
Color* yellow = new Color(254,255,0);
Color* black = new Color(0,0,0);
Color* green = new Color(0,254,0);

// Initialization routine.
void GL_init(void) 
{
    // Set background (or clearing) color.
    glClearColor(.0, .0, .0, 0.0); 

    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0);

    glDepthFunc(GL_LEQUAL);

    glewInit(); //Don't forget this guy
}
