#include "geomodeling.h"
#include <iostream>

bool tv_open = 1;

//窗户
void draw_window(void)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
       
    glPushMatrix();
    glTranslatef(-19.99, 0, 0);//-3,5
    glBegin(GL_QUADS);
    //left
    glColor4f(1.0, 1.0, 1.0, 0.1);
    glVertex3f(0, 4, -12);
    glVertex3f(0, 4,  -8);
    glVertex3f(0, 8,  -8);
    glColor4f(0.0, 0.0, 0.4, 0.1);
    glVertex3f(0, 8, -12);
    glEnd();
    glLineWidth(4);
    glColor3f(0.6f, 0.4f, 0.12f);

    glBegin(GL_LINES);
    //left
    glVertex3f(0.0f, 6, -12);
    glVertex3f(0.0f, 6, -8 );
    glVertex3f(0.0f, 8, -10);
    glVertex3f(0.0f, 4, -10);
    glEnd();
    glPopMatrix();
    glLineWidth(1);
    

}

//墙
void draw_wall(void)
{

    GLfloat x = 20;
    GLfloat y = 10;
    GLfloat z = 20;
    
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);					//back wall
    glColor3fv(hgreen);
    glVertex3f( x, y, -z);
    glVertex3f(-x, y, -z);

    glVertex3f(-x,-y, -z);
    glVertex3f( x,-y, -z);
    glEnd();

    //top wall
    glColor3fv(gray);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matRed);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matRed);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);
    glVertex3f( x, y, -z);
    glVertex3f( x, y,  0);
    glVertex3f(-x, y,  0);
    glVertex3f(-x, y, -z);
    glEnd();

    //left wall
    glMaterialfv(GL_FRONT, GL_AMBIENT, matGreen);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matGreen);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);
    glColor3fv(green);
    glVertex3f(-x, y, -z);
    glVertex3f(-x, y,  0);
    glColor3f(1, 1, 0);
    glVertex3f(-x,-y,  0);
    glVertex3f(-x,-y, -z);
    glEnd();

    //base wall
    glEnable(GL_TEXTURE_2D);//texture
    glColor3f(0, 0, 1);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matBlue);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBlue);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-x,-y, -z);
    glTexCoord2f(0.0, 1.0); glVertex3f(-x,-y,  0);
    glTexCoord2f(1.0, 1.0); glVertex3f( x,-y,  0);
    glTexCoord2f(1.0, 0.0); glVertex3f( x,-y, -z);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //right wall
    glMaterialfv(GL_FRONT, GL_AMBIENT, matYellow);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matYellow);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    glVertex3f( x,-y, -z);
    glVertex3f( x,-y,  0);
    glColor3f(0, 1, 0);
    glVertex3f( x, y,  0);
    glVertex3f( x, y, -z);
    glEnd();
    glPopMatrix();
}

//Celling Light
void draw_light(void)
{
    
    glPushMatrix();					//Celling Light

    glMaterialfv(GL_FRONT, GL_AMBIENT, matYellow);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matYellow);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    if(aCeilingLight) glMaterialfv(GL_FRONT, GL_EMISSION, matYellow);

    glTranslatef(0,10,-10);

    glScalef(5,0.5,5);
    glutSolidCube(1);
    glColor3f(1, 1, 0);
    glutWireCube(1);
    glPopMatrix();
}


//TV
void draw_tv(void)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);    
    
    //TV
    glPushMatrix();
    glTranslatef(18, 0, -7);
    glScalef( 0.3, 4, 6);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidCube(1);
    glLineWidth(4);
    glColor3f(0.0, 0.0, 1.0);
    glutWireCube(1);
    glLineWidth(1);
    glPopMatrix();
    
    if(tv_open){
        glPushMatrix();
        glTranslatef(17.72, -1.6, -9.65);
        glScalef(0.0009, 0.03, 0.014);
        glRotated(-90, 0, 1, 0);
        glColor3fv(red);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, 'H');
        glutStrokeCharacter(GLUT_STROKE_ROMAN, 'E');
        glutStrokeCharacter(GLUT_STROKE_ROMAN, 'L');
        glutStrokeCharacter(GLUT_STROKE_ROMAN, 'L');
        glutStrokeCharacter(GLUT_STROKE_ROMAN, 'O');
        glPopMatrix();
    }

}