#include "geomodeling.h"
#include "test3.h"
#include "test1.h"
#include <unistd.h>
#include<iostream>

bool lookTv = 0;
float myPII = 3.1415826;
GLfloat personX = -10;          // 初始坐标 -10， -5.5， -18
GLfloat personY = -5.5;         // 坐椅子坐标 15， -5.5, 8.5
GLfloat personZ = -18;
int personDirect = 0;
int personCondition = 0;   // 0站立， 1坐床边， 2弯腰，3竖躺床，4坐床上，5坐椅子
float personAngle = 0.0;
int timeGotoBed = 1;
int timeGotoSeat = 0;
int personGotoSeat = 0; // 0 走到床的左下角，1走到床的右下角，2走到椅子前面，3弯腰，4拉椅子，5坐下看电视

// 动画状态
float armSwingAngle = 0.0f;
float legSwingAngle = 0.0f;
float bodySwingAngle = 0.0;
bool armSwingDirection = true;
bool legSwingDirection = true;

// 定时器的间隔时间（毫秒）
const int personTimerInterval = 16;

void testcode3() {
    //draw_people();
    if (timeGotoBed) person_Goto_Bed(0);
    if (timeGotoSeat) person_Goto_Seat(0);
}

void Swing_Arm_Leg(int key) {      // 1 继续摆动，0恢复初始

    if (!key) {
        armSwingAngle = 0.0;
        legSwingAngle = 0.0;
        armSwingDirection = true;
        legSwingDirection = true;
    }

    // 更新手臂和腿的摆动角度
    if (armSwingDirection) {
        armSwingAngle += 0.001;
        if (armSwingAngle > 30.0f) {
            armSwingDirection = false;
        }
    } else {
        armSwingAngle -= 0.001;
        if (armSwingAngle < -30.0f) {
            armSwingDirection = true;
        }
    }

    if (legSwingDirection) {
        legSwingAngle += 0.002;
        if (legSwingAngle > 15.0f) {
            legSwingDirection = false;
        }
    } else {
        legSwingAngle -= 0.002;
        if (legSwingAngle < -15.0f) {
            legSwingDirection = true;
        }
    }
}

void person_Update_Direct(int x) {
    personDirect = x;
    personAngle = personDirect * 45;
}

void person_Update_Position() {
    if (personDirect == 0) personZ += 0.0001;
    if (personDirect == 2) personX += 0.0001;
    if (personDirect == 4) personZ -= 0.0001;
    if (personDirect == 6) personX -= 0.0001;
}

void person_Goto_Seat(int value) {

    if (!timeGotoSeat) return;

    if (personCondition == 4) {
        glutTimerFunc(
            3000,
            [](int) {
                personCondition = 1;
                glutPostRedisplay();
            },
            timeGotoSeat);
    }
    else if (personCondition == 1) {
        glutTimerFunc(
            3000,
            [](int) {
                personCondition = 0;
                glutPostRedisplay();
            },
            timeGotoSeat);
    }
    else if (personCondition == 0) {
        if (personGotoSeat == 0) {
            if (personX > -7.5) {
                person_Update_Direct(6);
                person_Update_Position();
                Swing_Arm_Leg(1);
                glutPostRedisplay();
                glutTimerFunc(personTimerInterval, person_Goto_Seat, 0);
            }
            else {
                if (personZ > -2) {
                    personGotoSeat = 1;
                    return;
                }
                person_Update_Direct(0);
                person_Update_Position();
                Swing_Arm_Leg(1);
                glutPostRedisplay();
                glutTimerFunc(personTimerInterval, person_Goto_Seat, 0);
            }
        }
        else if (personGotoSeat == 1) {
            if (personX > 8) {
                personGotoSeat = 2;
                return;
            }
            person_Update_Direct(2);
            person_Update_Position();
            Swing_Arm_Leg(1);
            glutPostRedisplay();
            glutTimerFunc(personTimerInterval, person_Goto_Seat, 0);
        }
        else if (personGotoSeat == 2) {
            if (personZ < -8) {
                bodySwingAngle = 0;
                personGotoSeat = 3;
                return;
            }
            person_Update_Direct(4);
            person_Update_Position();
            Swing_Arm_Leg(1);
            glutPostRedisplay();
            glutTimerFunc(personTimerInterval, person_Goto_Seat, 0);
        }
        else if (personGotoSeat == 3) {
            
            bodySwingAngle += 0.005;
            armSwingAngle += 0.008;

            if (bodySwingAngle >= 60) {
                personGotoSeat = 4;
                return;
            }

            person_Update_Direct(2);
            glutPostRedisplay();
            glutTimerFunc(personTimerInterval, person_Goto_Seat, 0);
        }
        else if (personGotoSeat == 4) {
            if (legSwingDirection) {
                legSwingAngle += 0.0002;
                if (legSwingAngle > 15.0f) {
                    legSwingDirection = false;
                }
            } else {
                legSwingAngle -= 0.0002;
                if (legSwingAngle < -15.0f) {
                    legSwingDirection = true;
                }
            }
            personX -= 0.00001;
            moveSeat += 0.00001;
            if (personX < 6.5) {
                personGotoSeat = 5;
                Swing_Arm_Leg(0);
                return;
            }
            glutPostRedisplay();
            glutTimerFunc(personTimerInterval, person_Goto_Seat, 0);
        }
        else if (personGotoSeat == 5) {
            if (!lookTv) {      
                glutTimerFunc(
                    1000,
                    [](int) {
                        lookTv = 1;
                        glutPostRedisplay();
                    },
                    timeGotoSeat);
            }
            else {
                if (armSwingAngle > 100) {
                    glutTimerFunc(
                        2000,
                        [](int) {
                            tv_open = 0;
                            personGotoSeat = 6;
                            glutPostRedisplay();
                        },
                        timeGotoSeat);
                }
                armSwingAngle += 0.001;
                glutPostRedisplay();
                glutTimerFunc(personTimerInterval, person_Goto_Seat, 0);
            }
        }
        else if (personGotoSeat == 6) {
            lookTv = 0;
            person_Update_Direct(0);
            Swing_Arm_Leg(0);
            glutPostRedisplay();
            glutTimerFunc(personTimerInterval, person_Goto_Seat, 0);
        }
    }
}

void person_Goto_Bed(int value) {

    if (!timeGotoBed) return;

    if (personZ < -8) {
        person_Update_Position();
        Swing_Arm_Leg(1);
        glutPostRedisplay();
        glutTimerFunc(personTimerInterval, person_Goto_Bed, timeGotoBed);
    }
    else if (personX < -6) {
        person_Update_Direct(2);
        person_Update_Position();
        Swing_Arm_Leg(1);
        glutPostRedisplay();
        glutTimerFunc(personTimerInterval, person_Goto_Bed, timeGotoBed);
    }
    else if (personCondition == 0) {
        personCondition = 1;
        person_Update_Direct(6);
        glutPostRedisplay();
        glutTimerFunc(personTimerInterval, person_Goto_Bed, timeGotoBed);
    }
    else if (personCondition == 1){
        glutTimerFunc(
            2000,
            [](int) {
                personCondition = 3;
                Swing_Arm_Leg(0);
                glutPostRedisplay();
            },
            timeGotoBed);
    }
    else if (personCondition == 3) {
        glutTimerFunc(
            1000,
            [](int) {
                bodySwingAngle = 0;
                personCondition = 2;
            },
            timeGotoBed);
    } 
    else if (personCondition == 2) {
        if (bodySwingAngle >= 88) {
            personCondition = 4;
            glutPostRedisplay();
            glutTimerFunc(0, NULL, timeGotoBed);
        }
        bodySwingAngle += 0.005;
        glutPostRedisplay();
        glutTimerFunc(personTimerInterval, person_Goto_Bed, 0);
    }
    else {
        timeGotoBed = 0;
        timeGotoSeat = 1;
        glutTimerFunc(100, NULL, timeGotoBed);
    }

    return;
}

void draw_people() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matBlack);
    glMaterialfv(GL_FRONT, GL_SHININESS, matBlack);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    if (Light2)
        glMaterialfv(GL_FRONT, GL_EMISSION, matBrown);

    /**********************start your code below**********************/

    glPushMatrix();
    glTranslatef(personX, personY, personZ);
    glRotatef(personAngle, 0.0f, 1.0f, 0.0f);

    if (personCondition == 1) { // 坐床边
        glTranslatef(0, -0.5, -2);
    }
    if (personGotoSeat == 5) {
        glTranslatef(0, -0.3, 5);
    }

    if (personCondition == 3 || personCondition == 4 || (timeGotoBed && personCondition == 2)) {
        glRotatef(-90, 1.0, 0, 0);
        glTranslated(-2, 3, -2);
        glRotatef(90, 0, 0, 1.0);
    }

    // 右腿
    glColor3f(0.7, 0.5, 0.5);
    glPushMatrix();
    if (personCondition == 1) {
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0.5, -1.2);
    } 
    else if (personGotoSeat == 5) {
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0.5, -1.2);
    } 
    else {
        glRotatef(legSwingAngle, 1, 0, 0);
    }
    glTranslatef(-0.6, -2.0, 0); // 大腿
    glPushMatrix();
    glScalef(0.8, 2.0, 0.6);
    glutSolidCube(1);
    glPopMatrix();

    if(personCondition == 1) {
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0.55, 0.7);
    }
    if (personGotoSeat == 5) {
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0.55, 0.7);
    }
    glTranslatef(0, -2, 0);  // 小腿
    glPushMatrix();
    glScalef(0.5, 2, 0.5);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);  // 鞋子
    glTranslatef(0, -0.76, 0);
    if (personCondition == 1) {
        glTranslatef(0, -0.5, 0);
    }
    if (personGotoSeat == 5) {
        glTranslatef(0, -0.5, 0);
    }
    glPushMatrix();
    glScalef(0.5, 0.5, 1.0);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    // 左腿
    glColor3f(0.7, 0.5, 0.5);
    glPushMatrix();
    if (personCondition == 1) {
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0.5, -1.2);
    } 
    else if (personGotoSeat == 5) {
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0.5, -1.2);
    } 
    else {
        glRotatef(-legSwingAngle, 1, 0, 0);
    }
    glTranslatef(0.6, -2.0, 0); // 大腿
    glPushMatrix();
    glScalef(0.8, 2.0, 0.6);
    glutSolidCube(1);
    glPopMatrix();

    if (personCondition == 1) { // 小腿
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0.62, 0.7);
    }
    if (personGotoSeat == 5) {
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, 0.62, 0.7);
    }
    glTranslatef(0, -1.75, 0);
    glPushMatrix();
    glScalef(0.5, 1.5, 0.5);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);  // 鞋子
    glTranslatef(0, -0.75, 0);
    if (personCondition == 1) {
        glTranslatef(0, -0.5, 0);
    }
    if (personGotoSeat == 5) {
        glTranslatef(0, -0.5, 0);
    }
    glPushMatrix();
    glScalef(0.5, 0.5, 1.0);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    // 身体
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    if (personCondition == 4 || (personCondition == 2 && timeGotoBed) ||
        personGotoSeat == 3 || personGotoSeat == 4) {
        glRotatef(bodySwingAngle, 1.0, 0, 0);
        glTranslatef(0, bodySwingAngle * myPII / 180 - 0.5,
                     bodySwingAngle * myPII / 180);
    }
    glScalef(2, 2, 0.6);
    glutSolidCube(1);
    // 扣子
    glColor3f(0.7, 0.5, 0.5);
    glPointSize(4);
    glBegin(GL_POINTS);
    glVertex3f(0, 0.25, 0.6);
    glVertex3f(0, -0.25, 0.6);
    glEnd();
    glPopMatrix();

    // 脖子
    glColor3ub(239, 228, 176);
    glPushMatrix();
    if (personCondition == 4 || (personCondition == 2 && timeGotoBed) ||
        personGotoSeat == 3 || personGotoSeat == 4) {
        glRotatef(bodySwingAngle, 1.0, 0, 0);
        glTranslatef(0, bodySwingAngle * myPII / 180 - 0.5,
                     bodySwingAngle * myPII / 180);
    }
    glTranslatef(0, 1.2, 0);
    glScalef(0.7, 0.4, 0.6);
    glutSolidCube(1);
    glPopMatrix();

    // 右胳膊
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    if (personCondition == 4 || (personCondition == 2 && timeGotoBed) ||
        personGotoSeat == 3 || personGotoSeat == 4) {
        glRotatef(bodySwingAngle, 1.0, 0, 0);
        glTranslatef(0, bodySwingAngle * myPII / 180 - 0.5,
                     bodySwingAngle * myPII / 180);
    }
    if (personGotoSeat == 4) {
        glRotatef(-armSwingAngle, 1, 0, 0);
        glTranslatef(0, -1, 1);
    }
    else if (personGotoSeat == 5 && !lookTv) {
        glRotatef(-armSwingAngle, 1, 0, 0);
    }
    else if (personGotoSeat == 5 && lookTv) {
        glTranslatef(0, 1, 1);
        glRotatef(-armSwingAngle, 1, 0, 0);
    }
    else {
        glRotatef(armSwingAngle, 1, 0, 0); // 根据 armAngle 旋转
    }
    glTranslatef(-1.25, 0.5, 0);
    glPushMatrix();
    glScalef(0.5, 1, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    glTranslatef(0, -1, 0);
    glPushMatrix();
    glScalef(0.4, 1, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    glColor3ub(239, 228, 176);
    glTranslatef(0, -0.75, 0);
    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    // 左胳膊
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    if (personCondition == 4 || (personCondition == 2 && timeGotoBed) ||
        personGotoSeat == 3 || personGotoSeat == 4) {
        glRotatef(bodySwingAngle, 1.0, 0, 0);
        glTranslatef(0, bodySwingAngle * myPII / 180 - 0.5,
                     bodySwingAngle * myPII / 180);
    }
    if (personGotoSeat == 3) {
        glRotatef(armSwingAngle, 1, 0, 0);
    } 
    else if (personGotoSeat == 4) {
        glRotatef(-armSwingAngle, 1, 0, 0);
        glTranslatef(0, -1, 1);
    }
    else {
        glRotatef(-armSwingAngle, 1, 0, 0);
    }
    glTranslatef(1.25, 0.5, 0);
    glPushMatrix();
    glScalef(0.5, 1, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    glTranslatef(0, -1, 0);
    glPushMatrix();
    glScalef(0.4, 1, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    glColor3ub(239, 228, 176);
    glTranslatef(0, -0.75, 0);
    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    // 头
    glColor3ub(239, 228, 176);
    glPushMatrix();
    if (personCondition == 4 || (personCondition == 2 && timeGotoBed) ||
        personGotoSeat == 3 || personGotoSeat == 4) {
        glRotatef(bodySwingAngle, 1.0, 0, 0);
        glTranslatef(0, bodySwingAngle * myPII / 180 - 0.5,
                     bodySwingAngle * myPII / 180);
    }
    glTranslatef(0, 2.4, 0);
    glScalef(0.8, 1.0, 0.7);
    glutSolidSphere(1, 100, 100);

    // 左眼
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(4);
    glBegin(GL_POINTS);
    glVertex3f(0.4, 0.4, 1); // 左眼的位置
    glEnd();

    // 右眼
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(4);
    glBegin(GL_POINTS);
    glVertex3f(-0.4, 0.4, 1); // 右眼的位置
    glEnd();

    // 微笑的嘴巴（作为一个点）
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(4);
    glBegin(GL_POINTS);
    // 计算嘴巴位置
    float angle = 15.0f * 3.14159f / 180.0f; // 15度转换为弧度
    float mouthX = 0.0f;                     // 在YOZ平面中，X为0
    float mouthY = -sin(angle);              // 计算Y位置
    float mouthZ = cos(angle);               // 计算Z位置
    glVertex3f(mouthX, mouthY, mouthZ);      // 绘制嘴巴的位置
    glEnd();


    glPopMatrix();
    

    glPopMatrix();

    /***********************end your code here************************/
}
