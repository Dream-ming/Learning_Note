#include "geomodeling.h"
#include "test2.h"
#include <time.h>

void testcode2(){
    draw_clock();
    updateClock(0);
}

void updateClock(int value) {
    glutTimerFunc(1000, updateClock, 0);
}

/*
圆盘形石英钟（内外圆盘叠加）： 位置：(0, 4, -19.9); 
外盘：inr = 3, outr = 3.5，glColor3f(1, 0.6, 0.3); gluDisk(obj, inr, outr, 20, 4);
内盘：glColor3f(0.25, 0.25, 0.25); gluDisk(obj, 0, inr, 20, 4);
刻度：整点刻度长1，线宽4，其它刻度长0.5，线宽2，颜色：白色
时针：起点为表盘中心点，长度为内径inr的0.5倍，线宽3，颜色：白色
分针：起点为表盘中心点，长度为内径inr的0.6倍，线宽3，颜色：白色
秒针：起点为表盘中心点，长度为内径inr的0.7倍，线宽3，颜色：白色
注：获取系统时间，并预留线下时针、分针、秒针随系统时间运动的功能。头歌线上提交时，表盘时间为3（时）：0（分）：0（秒）.
*/

// 提示：在合适的地方修改或添加代码
void draw_clock(void) {
    GLUquadric *obj;
    obj = gluNewQuadric();

    glColor3f(1, 0.6, 0.3);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matYellow);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matYellow);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    //========begin========

    glPushMatrix();

    // 内外表盘
    glPushMatrix();
    glColor3f(1.0f, 0.6f, 0.3f);
    glTranslatef(0.0f, 4.0f, -19.9f);
    gluDisk(obj, 3.0, 3.5, 20, 4);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.25, 0.25, 0.25);
    glTranslatef(0.0f, 4.0f, -19.8f);
    gluDisk(obj, 0, 3.0, 20, 4);
    glPopMatrix();

    // 刻度
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 720; ++i) {
        glPushMatrix();
        glTranslatef(0.0f, 4.0f, -19.7f);
        glRotatef(i * 0.5f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        if (i % 60 == 0) {
            glLineWidth(4.0f);
            glVertex2f(0.0f, 3.0f);
            glVertex2f(0.0f, 2.0f);
        } else {
            glLineWidth(2.0f);
            glVertex2f(0.0f, 3.0f);
            glVertex2f(0.0f, 2.5f);
        }
        glEnd();
        glPopMatrix();
    }

    // 获取系统时间
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    int hours = local->tm_hour;
    int minutes = local->tm_min;
    int seconds = local->tm_sec;

    // 将时间调整为12小时制
    hours = hours % 12;

    // 计算旋转角度
    float hourAngle = (hours + minutes / 60.0f) * 30.0f;    // 每小时30度
    float minuteAngle = (minutes + seconds / 60.0f) * 6.0f; // 每分钟6度
    float secondAngle = seconds * 6.0f;                     // 每秒6度

    glTranslatef(0, 4, -19.6);
    glLineWidth(1);
    glColor3f(1.0, 1.0, 1.0);
    
    // 绘制时针
    glPushMatrix();
    glRotatef(-hourAngle, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 1.5f); // 时针长度为内径的0.5倍
    glEnd();
    glPopMatrix();

    // 绘制分针
    glPushMatrix();
    glRotatef(-minuteAngle, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 1.8f); // 分针长度为内径的0.6倍
    glEnd();
    glPopMatrix();

    // 绘制秒针
    glPushMatrix();
    glRotatef(-secondAngle, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 2.1f); // 秒针长度为内径的0.7倍
    glEnd();
    glPopMatrix();

    glPopMatrix();

    //=========end=========
    gluDeleteQuadric(obj);
}
