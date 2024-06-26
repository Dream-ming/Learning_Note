#ifndef __GEOMODELING_H__
#define __GEOMODELING_H__

#include <windows.h>
#include <GL/freeglut.h>
#include <cmath>
//#include <GL/glut.h>
#include <ctime>

#define Pi 3.1415926

static float gold[3]={1,1,0.2},brown[3]={0.7,0.5,0.5}, hgreen[3]={0,1,0.2},blue[3]={0,0,1},white[3]={1,1,1},black[3]={0,0,0},hgray[3]={0.6,0.6,0.6},red[3]={1,0,0},gray[3]={0.5,0.5,0.5},green[3]={0,1,0};
static float matWhite[4]={1,1,1,1},matBlack[4]={0,0,0,0},matBrown[4]={0.7,0.5,0.5,1},matYellow[4]={0,1,1,1},matRed[4]={1,0,0,1}, matGreen[4]={0,1,0,1},matBlue[4]={0,0,1,1};
static float pep_agl = 0, pep_ag2 = 0, clock_agl1=0, clock_agl2=0, seat_dis=1, matShininess = 30.0;
static bool Light0=0,Light1=0,Light2=0,Light3=0,aCeilingLight=1, pep_retot=1;
extern bool tv_open;

void draw_wall();
void draw_window();
void draw_light();
void draw_tv();

#endif