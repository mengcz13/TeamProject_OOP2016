#ifndef DISPLAY_H
#define DISPLAY_H
#include <cstdlib>
#include <GL/glut.h>
#include "Ttree.h"

// Task vector
Task* taskvec;
int tasknum;

// all variables initialized to 1.0, meaning
// the triangle will initially be white
float red=1.0f, blue=1.0f, green=1.0f;

// angle for rotating triangle
float angle = 0.0f;
float angle2 = 0.0f;
float angle3 = 0.0f;

void drawWireBlock(double ox, double oy, double oz, double x, double y, double z) {
    glColor3f(red,green,blue);
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

void renderScene(void) {

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera
    gluLookAt(  0.0f, 0.0f, 10.0f,
            0.0f, 0.0f,  0.0f,
            0.0f, 1.0f,  0.0f);

    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glRotatef(angle2, 1.0f, 0.0f, 0.0f);
    glRotatef(angle3, 0.0f, 0.0f, 1.0f);

    //drawWireBlock(0, 0, 0, 1, 2, 3);
    for (int i = 0; i < tasknum; ++i) {
        drawWireBlock(taskvec[i].o.x / 5, taskvec[i].o.y/5, taskvec[i].o.t/5, taskvec[i].X/5, taskvec[i].Y/5, taskvec[i].T/5);
    }

    // glColor3f(red,green,blue);
    // glBegin(GL_TRIANGLES);
    //  glVertex3f(-2.0f,-2.0f, 0.0f);
    //  glVertex3f( 2.0f, 0.0f, 0.0);
    //  glVertex3f( 0.0f, 2.0f, 0.0);
    // glEnd();

    //angle+=0.1f;

    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27)
        exit(0);
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

void showtask(Task* taskvec2, int tnum) {
    taskvec = taskvec2;
    tasknum = tnum;
    // init GLUT and create window
    int argc = 1;
    glutInit(&argc, NULL);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,600);
    glutCreateWindow("Display");

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // here are the new entries
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    // enter GLUT event processing cycle
    glutMainLoop();
}

#endif