// 提示：在合适的地方修改或添加代码
#include "test1.h"
#include "test2.h"
#include "test3.h"
#include "geomodeling.h"

float moveSeat = 0;

void testcode1() {
    draw_tvdesk();
    draw_seat();
    draw_bed();
    draw_closet();
    draw_taideng();
    draw_clock();
    draw_people();
}

void testanimation(int value) {
    /****here,add glut callback events for animation*/

    glutPostRedisplay();
    glutTimerFunc(1000, testanimation, 0);

    /*********************end***********************/
    return;
}

// bed
void draw_bed(void) {

    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    /********** begin your code below*************/

    // 绘制床头
    glColor3f(0.f, 0.54f, 0.5f);
    glPushMatrix();
    glTranslatef(0, -7, -19.5);
    glScalef(12, 6, 1);
    glutSolidCube(1);
    glPopMatrix();
    // 绘制床头白框
    glLineWidth(2.0f);
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(0, -7, -19.5);
    glScalef(12, 6, 1);
    glutWireCube(1);
    glPopMatrix();

    // 绘制床身
    glColor3f(1.f, 0.54f, 0.5f);
    glPushMatrix();
    glTranslatef(0, -9.25f, -10.5f);
    glScalef(12, 1.5f, 14);
    glutSolidCube(1);
    glPopMatrix();
    // 床身白框
    glLineWidth(2);
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(0, -9.25f, -10.5f);
    glScalef(12, 1.5f, 14);
    glutWireCube(1);
    glPopMatrix();

    /***********end your code here****************/
}

// 台灯+床头柜
void draw_taideng(void) {
    GLUquadric *obj;
    obj = gluNewQuadric();
    glColor3f(1, 0, 1);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matBlack);
    glMaterialfv(GL_FRONT, GL_SHININESS, matBlack);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    if (Light2)
        glMaterialfv(GL_FRONT, GL_EMISSION, matBrown);

    /********** begin your code below*************/

    // 床头柜
    glPushMatrix();
    glTranslatef(8, -8.5f, -18.5f);
    glScalef(3.0f, 3, 3.0f);
    glColor3f(1, 0.6f, 0.3f);
    glutSolidCube(1);
    glPopMatrix();

    // 黑边
    glLineWidth(2);
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(8, -8.5, -18.5);
    glScalef(3.0, 3.0, 3.0);
    glutWireCube(1);
    glPopMatrix();

    // 柜把手
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex3f(7.5f, -8.5f, -16.99);
    glVertex3f(8.5f, -8.5f, -16.99);
    glEnd();

    // 灯罩
    glColor3f(1, 0, 1);
    glPushMatrix();
    glTranslatef(8, -4.5f, -18.5f);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(1.5f, 2, 16, 16);
    glPopMatrix();

    // 灯柱
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(8, -7, -18.5f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(obj, 0.5f, 0.2f, 4, 40, 40);
    glPopMatrix();

    /***********end your code here****************/

    gluDeleteQuadric(obj);
}

void draw_tvdesk(void) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    // TV desk
    /********** begin your code below*************/

    glColor3f(1, 0.6, 0.3);

    // 后侧
    glPushMatrix();
    glTranslatef(18.2f, -7, -13.2f);
    glScalef(3.6f, 6, 0.4f);
    glutSolidCube(1);
    glPopMatrix();
    // 前侧
    glPushMatrix();
    glTranslatef(18.2f, -7, -5.2f);
    glScalef(3.6f, 6, 0.4f);
    glutSolidCube(1);
    glPopMatrix();
    // 桌面
    glPushMatrix();
    glTranslatef(18.2f, -4.2f, -9.2f);
    glScalef(3.6f, 0.4f, 8.4f);
    glutSolidCube(1);
    glPopMatrix();
    // 层板
    glPushMatrix();
    glTranslatef(18.2f, -7.4f, -9.2f);
    glScalef(3.6f, 0.4f, 8.4f);
    glutSolidCube(1);
    glPopMatrix();
    // 上中侧
    glPushMatrix();
    glTranslatef(18.2f, -5.8f, -9.3f);
    glScalef(3.6f, 3.6f, 0.4f);
    glutSolidCube(1);
    glPopMatrix();

    /***********end your code here****************/
}

// seat
void draw_seat(void) {

    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    GLUquadric *obj;
    obj = gluNewQuadric();

    /********** begin your code below*************/

    glTranslatef(-moveSeat, 0, 0);

    // 椅子腿
    glPushMatrix();
    glTranslatef(11.5f, -10, -8.5f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(obj, 0.2f, 0.2f, 2.5f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(11.5f, -10, -6.5f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(obj, 0.2f, 0.2f, 2.5f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(13.5f, -10, -6.5f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(obj, 0.2f, 0.2f, 2.5f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(13.5f, -10, -8.5f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(obj, 0.2, 0.2f, 2.5f, 10, 10);
    glPopMatrix();

    // 凳子面
    glPushMatrix();
    glTranslatef(12.5f, -7.25f, -7.5f);
    glScalef(3, 0.5f, 3);
    glutSolidCube(1);
    glPopMatrix();

    glTranslatef(moveSeat, 0, 0);

    /***********end your code here****************/
    gluDeleteQuadric(obj);
}

// closet
void draw_closet(void) {

    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    /********** begin your code below*************/

    glColor3f(1, 0.6f, 0.3f);
    glPushMatrix();
    glTranslatef(-18, -3, -17);
    glScalef(4, 14, 6);
    glPushMatrix();
    glutSolidCube(1);

    // 黑线
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3f(0.5f, 0.5f, 0);
    glVertex3f(0.5f, -0.5f, 0);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 0.0f);
    // 黑框
    glLineWidth(2);
    glPushMatrix();
    glTranslatef(-18, -3, -17);
    glScalef(4, 14, 6);
    glutWireCube(1);
    glPopMatrix();

    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(-15.9f, -3, -17.6f);
    glScalef(0.2f, 2, 0.2f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-15.9f, -3, -16.4f);
    glScalef(0.2f, 2, 0.2f);
    glutSolidCube(1);
    glPopMatrix();

    /***********end your code here****************/
}