// 评测代码所用头文件-开始
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
// 评测代码所用头文件-结束

// 提示：写完代码请保存之后再进行评测
#include <GL/freeglut.h>
#include <stdio.h>

#define LEFT   1
#define RIGHT  2
#define BOTTOM 4
#define TOP    8

struct MyRect
{
	int xmin, xmax, ymin, ymax;
    MyRect() : xmin(), xmax(), ymin(), ymax(){};
    MyRect(int a, int b, int c, int d) : xmin(a), xmax(b), ymin(c), ymax(d){};
};

struct Point
{
	int x, y;
	Point() : x(), y() {};
	Point(int a, int b) :x(a), y(b) {};
};

MyRect  rect;
Point vPoint[6];

void LineGL(Point node1, Point node2)
{
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);   
    glVertex2f(node1.x, node1.y);
	glVertex2f(node2.x, node2.y);
	glEnd();
}

int CompCode(Point node, MyRect rect)
{
	// 请在此添加你的代码
   /********** Begin ********/

    int res = 0;

    if (node.x < rect.xmin) res |= LEFT;
    else if (node.x > rect.xmax) res |= RIGHT;

    if (node.y < rect.ymin) res |= BOTTOM;
    else if (node.y > rect.ymax) res |= TOP;

    return res;

   /********** End **********/
}

void LineClipCohenSurtherland(MyRect rect, Point& node1, Point& node2)
{
	bool accept = true;
	// 请在此添加你的代码
   /********** Begin ********/

    int r1 = CompCode(node1, rect);
    int r2 = CompCode(node2, rect);

    while (1) {
        if ((r1 | r2) == 0) {
            break;
        }
        else if (r1 & r2) {
            accept = false;
            break;
        }
        else {
            int r = (r1 == 0) ? r2 : r1;
            Point node;
            if (r & LEFT) {
                node.x = rect.xmin;
                node.y = node1.y + (node2.y - node1.y) * (rect.xmin - node1.x) / (node2.x - node1.x);
            }
            else if (r & RIGHT) {
                node.x = rect.xmax;
                node.y = node1.y + (node2.y - node1.y) * (rect.xmax - node1.x) / (node2.x - node1.x);
            }
            else if (r & BOTTOM) {
                node.y = rect.ymin;
                node.x = node1.x + (node2.x - node1.x) * (rect.ymin - node1.y) / (node2.y - node1.y);
            }
            else if (r & TOP) {
                node.y = rect.ymax;
                node.x = node1.x + (node2.x - node1.x) * (rect.ymax - node1.y) / (node2.y - node1.y);
            }

            if (r == r1) {
                node1 = node;
                r1 = CompCode(node1, rect);
            }
            else {
                node2 = node;
                r2 = CompCode(node2, rect);
            }
        }
    }


   /********** End **********/
	if (accept)
        LineGL(node1, node2);
}

void MyDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectf(rect.xmin, rect.ymin, rect.xmax, rect.ymax);

  	for (int i = 0; i < 5; i+=2) 
		LineClipCohenSurtherland(rect, vPoint[i], vPoint[i+1]);
	
	
	glFlush();
}

void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

    rect = MyRect(100, 300, 100, 300);

	vPoint[0] = Point(200, 50); vPoint[1] = Point(350, 250);
	vPoint[2] = Point(125, 205); vPoint[3] = Point(250, 255);
	vPoint[4] = Point(40, 150); vPoint[5] = Point(150, 40);
}

void MyReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Hello World!");

	Init();
	glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);   
    glutMainLoopEvent(); 


    /*************以下为评测代码，与本次实验内容无关，请勿修改**************/
	GLubyte* pPixelData = (GLubyte*)malloc(400 * 400 * 3);//分配内存
    GLint viewport[4] = {0};    
    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(viewport[0], viewport[1], viewport[2], viewport[3], GL_RGB, GL_UNSIGNED_BYTE, pPixelData);

	cv::Mat img;
    std::vector<cv::Mat> imgPlanes;
    img.create(400, 400, CV_8UC3);
    cv::split(img, imgPlanes);
 
    for(int i = 0; i < 400; i ++) {
        unsigned char* plane0Ptr = imgPlanes[0].ptr<unsigned char>(i);
        unsigned char* plane1Ptr = imgPlanes[1].ptr<unsigned char>(i);
        unsigned char* plane2Ptr = imgPlanes[2].ptr<unsigned char>(i);
        for(int j = 0; j < 400; j ++) {
            int k = 3 * (i * 400 + j);
            plane2Ptr[j] = pPixelData[k];
            plane1Ptr[j] = pPixelData[k+1];
            plane0Ptr[j] = pPixelData[k+2];
        }
    }
    cv::merge(imgPlanes, img);
    cv::flip(img, img ,0); 
    cv::namedWindow("openglGrab");
    cv::imshow("openglGrab", img);
    cv::imwrite("../img_step1/test.jpg", img);
	return 0;
}
