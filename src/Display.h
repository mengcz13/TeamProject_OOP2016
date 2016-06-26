#ifndef DISPLAY_H
#define DISPLAY_H
#include <cstdlib>
// Linux headers
#ifdef __gnu_linux__
#include <GL/glut.h>
#endif
// mac headers
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "Ttree.h"
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

// Task vector
Task* taskvec;
int tasknum;
double bx, by, bt;

// all variables initialized to 1.0, meaning
// the triangle will initially be white
float red=1.0f, blue=1.0f, green=1.0f;

// angle for rotating triangle
float angle = 0.0f;
float angle2 = 0.0f;
float angle3 = 0.0f;

float scale = 1;

void drawLines(double ox, double oy, double oz, double x, double y , double z) {
    glBegin(GL_LINES);
    glVertex3f(ox, oy, oz);
    glVertex3f(ox + x, oy, oz);
    glVertex3f(ox, oy + y, oz);
    glVertex3f(ox + x, oy + y, oz);
    glVertex3f(ox, oy, oz + z);
    glVertex3f(ox + x, oy, oz + z);
    glVertex3f(ox, oy + y, oz + z);
    glVertex3f(ox + x, oy + y, oz + z);
    
    glVertex3f(ox, oy, oz);
    glVertex3f(ox, oy + y, oz);
    glVertex3f(ox + x, oy, oz);
    glVertex3f(ox + x, oy + y, oz);
    glVertex3f(ox, oy, oz + z);
    glVertex3f(ox, oy + y, oz + z);
    glVertex3f(ox + x, oy, oz + z);
    glVertex3f(ox + x, oy + y, oz + z);
    
    glVertex3f(ox, oy, oz);
    glVertex3f(ox, oy, oz + z);
    glVertex3f(ox + x, oy, oz);
    glVertex3f(ox + x, oy, oz + z);
    glVertex3f(ox, oy + y, oz);
    glVertex3f(ox, oy + y, oz + z);
    glVertex3f(ox + x, oy + y, oz);
    glVertex3f(ox + x, oy + y, oz + z);
    glEnd();
}

void drawWireBlock(Task& t) {
    double ox = t.o.x*scale, oy = t.o.y*scale, oz = t.o.t*scale;
    double x = t.X*scale, y = t.Y*scale, z = t.T*scale;
    glColor3f(red,green,blue);
    
    drawLines(ox, oy, oz, x, y, z);
}

void drawContainer() {
    glColor3f(red,0.0f,blue);
    
    drawLines(0, 0, 0, bx*scale, by*scale, bt*scale);
}

void drawFaceBlock(Task& t) {
    double ox = t.o.x*scale, oy = t.o.y*scale, oz = t.o.t*scale;
    double x = t.X*scale, y = t.Y*scale, z = t.T*scale;
    
    glPushMatrix();
    glTranslatef(ox,oy,oz);
    glBegin(GL_QUADS);    //顶面
    glNormal3f(0.0f,2.0f,0.0f);
    glVertex3f(x,y,z);
    glVertex3f(x,y,0.0f);
    glVertex3f(0.0f,y,0.0f);
    glVertex3f(0.0f,y,z);
    glEnd();
    glBegin(GL_QUADS);    //底面
    glNormal3f(0.0f,-2.0f,0.0f);
    glVertex3f(x,0.0f,z);
    glVertex3f(0.0f,0.0f,z);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(x,0.0f,0.0f);
    glEnd();
    glBegin(GL_QUADS);    //前面
    glNormal3f(0.0f,0.0f,2.0f);
    glVertex3f(x,y,z);
    glVertex3f(0.0f,y,z);
    glVertex3f(0.0f,0.0f,z);
    glVertex3f(x,0.0f,z);
    glEnd();
    glBegin(GL_QUADS);    //背面
    glNormal3f(0.0f,0.0f,-2.0f);
    glVertex3f(x,y,0.0f);
    glVertex3f(x,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,y,0.0f);
    glEnd();
    glBegin(GL_QUADS);    //左面
    glNormal3f(-2.0f,0.0f,0.0f);
    glVertex3f(0.0f,y,z);
    glVertex3f(0.0f,y,0.0f);
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(0.0f,0.0f,z);
    glEnd();
    glBegin(GL_QUADS);    //右面
    glNormal3f(2.0f,0.0f,0.0f);
    glVertex3f(x,y,z);
    glVertex3f(x,0.0f,z);
    glVertex3f(x,0.0f,0.0f);
    glVertex3f(x,y,0.0f);
    glEnd();
    glPopMatrix();
    
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio =  w * 1.0 / h;

        // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

        // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

const GLfloat PI = 3.14;

// record the state of mouse
GLboolean mouserdown = GL_FALSE;
GLboolean mouseldown = GL_FALSE;
GLboolean mousemdown = GL_FALSE;

// when a mouse-key is pressed, record current mouse position
static GLint mousex = 0, mousey = 0;

static GLfloat center[3] = {0.0f, 0.0f, 0.0f}; // center position
static GLfloat eye[3]; // eye's position

static GLfloat yrotate = PI/4; // angle between y-axis and look direction
static GLfloat xrotate = PI/4; // angle between x-axis and look direction
static GLfloat celength = 20.0f;// lenght between center and eye

static GLfloat mSpeed = 0.4f; // center move speed
static GLfloat rSpeed = 0.02f; // rotate speed
static GLfloat lSpeed = 0.4f; // reserved

// calculate the eye position according to center position and angle,length
void CalEyePosition() {
    if(yrotate > PI) yrotate = PI; // restrict rotation
    if(yrotate < 0.01)  yrotate = 0.01;
    if(xrotate > 2*PI)   xrotate = 0.01;
    if(xrotate < 0)   xrotate = 2 * PI; 
    if(celength > 50)  celength = 50; // restrict scaling
    if(celength < 5)   celength = 5;
    eye[0] = center[0] + celength * sin(yrotate) * cos(xrotate);
    eye[2] = center[2] + celength * sin(yrotate) * sin(xrotate);
    eye[1] = center[1] + celength * cos(yrotate);
}

void MouseFunc(int button, int state, int x, int y) {
    if(state == GLUT_DOWN) {
        if(button == GLUT_RIGHT_BUTTON) mouserdown = GL_TRUE;
        if(button == GLUT_LEFT_BUTTON) mouseldown = GL_TRUE;
        if(button == GLUT_MIDDLE_BUTTON)mousemdown = GL_TRUE;
    }
    else {
        if(button == GLUT_RIGHT_BUTTON) mouserdown = GL_FALSE;
        if(button == GLUT_LEFT_BUTTON) mouseldown = GL_FALSE;
        if(button == GLUT_MIDDLE_BUTTON)mousemdown = GL_FALSE;
    }
    mousex = x, mousey = y;
}

void MouseMotion(int x, int y) {
    if(mouserdown == GL_TRUE) {
        xrotate += (x - mousex) / 80.0f;
        yrotate -= (y - mousey) / 120.0f;
    }
    
    if(mouseldown == GL_TRUE) {
        celength += (y - mousey) / 25.0f;
    }
    mousex = x, mousey = y;
    CalEyePosition();
    glutPostRedisplay();
}

void renderScene(void) {
    

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera
    
    CalEyePosition();
    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0, 1, 0);
    
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glRotatef(angle2, 1.0f, 0.0f, 0.0f);
    glRotatef(angle3, 0.0f, 0.0f, 1.0f);

    //drawWireBlock(0, 0, 0, 1, 2, 3);
    glEnable(GL_LIGHT0);
    for (int i = 0; i < tasknum; ++i) {
        drawFaceBlock(taskvec[i]);
    }
    glDisable(GL_LIGHT0);
    for (int i = 0; i < tasknum; ++i) {
        drawWireBlock(taskvec[i]);
    }
    drawContainer();

    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27)
        exit(0);
    if (key == 'l') {
        GLint params[] = {GL_POLYGON_MODE, GL_FILL};
        glGetIntegerv(GL_POLYGON_MODE, params);
        if (*(params) == GL_FILL) {
            glPolygonMode(GL_FRONT_AND_BACK ,GL_POINT);
            glDisable(GL_LIGHTING);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK ,GL_FILL);
            glEnable(GL_LIGHTING);
        }
    }
}

void processSpecialKeys(int key, int x, int y) {

    switch(key) {
        case GLUT_KEY_F1 :
                angle += 5.0f; break;
        case GLUT_KEY_F2 :
                angle2 += 5.0f;break;
        case GLUT_KEY_F3 :
                angle3 += 5.0f;break;
    }
}

// light variables
float ambientLight[]={0.1f,0.3f,0.8f,1.0f};
float diffuseLight[]={0.25f,0.25f,0.25f,1.0f};
float lightPosition[]={0.0f,0.0f,1.0f,0.0f};
// material variables
float matAmbient[]={1.0f,1.0f,1.0f,0.7f};
float matDiff[]={1.0f,1.0f,1.0f,0.7f};

void showtask(string fname) {
    ifstream resultfile(fname);
    
    resultfile >> bx >> by >> bt >> tasknum;
    
    taskvec = new Task[tasknum];
    for (int i = 0; i < tasknum; ++i) {
        resultfile >> taskvec[i].X >> taskvec[i].Y >> taskvec[i].T;
        resultfile >> taskvec[i].o.x >> taskvec[i].o.y >> taskvec[i].o.t;
    }
    resultfile.close();
    // init GLUT and create wi ndow
    int argc = 1;
    char *argv[] = {"a", "b"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,600);
    glutCreateWindow("Display");
    
    // antialiasing
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    
    //light and material
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    
    glPolygonMode(GL_FRONT_AND_BACK ,GL_POINT);
    glDisable(GL_LIGHTING);
    
    glMaterialfv(GL_FRONT,GL_AMBIENT,matAmbient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,matDiff);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    
    glutMouseFunc(MouseFunc);
    glutMotionFunc(MouseMotion);

    // here are the new entries
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    // enter GLUT event processing cycle
    glutMainLoop();
}

#endif