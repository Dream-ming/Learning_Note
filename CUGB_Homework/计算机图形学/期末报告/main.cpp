#include <GL/freeglut.h>
// #include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "test1.h"
#include "test2.h"
#include "test3.h"
#include "geomodeling.h"

using namespace std;

int windowWidth = 600;   // 窗口宽度
int windowHeight = 350;  // 窗口高度

int WinWidth,WinHeight;
static unsigned int texName;
unsigned char checkImage[64][64][4];
static int checkImageHeight = 64, checkImageWidth = 64;
static float eye[3] = { 0,0,20 }, eobj[3] = { 0,0,0 };

//产生纹理
void makeCheckImages(void)
{
    int i, j, c;
    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
            if (c == 255)
            {
                checkImage[i][j][0] = (GLubyte)180;
                checkImage[i][j][1] = (GLubyte)180;
                checkImage[i][j][2] = (GLubyte)180;
                checkImage[i][j][3] = (GLubyte)255;
            }
            else
            {
                checkImage[i][j][0] = (GLubyte)c;
                checkImage[i][j][1] = (GLubyte)c;
                checkImage[i][j][2] = (GLubyte)c;
                checkImage[i][j][3] = (GLubyte)255;
            }
        }
    }
}

void setLight0(bool Light0)
{
    Light0 = 0;

    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_position[] = { -2.0, 2.0, 1.0, 1.0 };
    GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    glEnable(GL_LIGHT1);
}

void updateCamera() {

    if (lookTv == 0) {
        eye[0] = 0;
        eye[1] = 0;
        eye[2] = 20;
        eobj[0] = 0;
        eobj[1] = 0;
        eobj[2] = 0;
    } 
    else if (lookTv == 1) {
        eye[0] = 12.5, eye[1] = -4.5f, eye[2] = -9.5f;
        eobj[0] = 20.0f, eobj[1] = -0.5f, eobj[2] = -6;
    }
    else if (lookTv == 2) {
        eye[0] = 0;
        eye[1] = 0;
        eye[2] = 20;
    }
}

void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
    if (h == 0) h = 1;
    WinWidth = w;
    WinHeight = h;

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    gluLookAt(eye[0], eye[1], eye[2], eobj[0], eobj[1], eobj[2], 0.0, 1.0, 0.0);

}

void display(void) // Here's Where We Do All The Drawing
{

    updateCamera();
    reshape(WinWidth, WinHeight);
    // TODO:
    // Place light source here
    setLight0(Light0);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//texture

    // TODO:
    // Draw walls and objects here
    draw_wall();
    draw_window();
    draw_light();
    draw_tv();

    testcode1();
    testcode2();
    testcode3();

    glutSwapBuffers();

}




void init(void) 
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    //texture mapping
    makeCheckImages();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(1000, timer, 0);
}

int main(int argc, char** argv) {

    int width = 600, height = 350;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Add furnitures");

    init();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);   // 调用display函数

    glutTimerFunc(0, testanimation, 0); // 设置定时器

    glutMainLoop();

    return 0;
}

