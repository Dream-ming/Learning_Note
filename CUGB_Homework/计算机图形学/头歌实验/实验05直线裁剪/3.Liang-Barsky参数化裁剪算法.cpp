// 评测代码所用头文件-开始
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
// 评测代码所用头文件-结束

// 提示：写完代码请保存之后再进行评测
#include <GL/freeglut.h>
#include <stdio.h>

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

int ClipTest(float p, float q, float* u1, float* u2)
{
   	// 请在此添加你的代码
   /********** Begin ********/

    if (p < 0.0) {
        float r = q / p;
        if (r > *u2) {
            return 0;
        }
        else if (r > *u1) {
            *u1 = r;
        }
    }
    else if (p > 0.0) {
        float r = q / p;
        if (r < *u1) {
            return 0;
        }
        else if (r < *u2) {
            *u2 = r;
        }
    }
    else if (q < 0.0) {
        return 0;
    }
    return 1;

   /********** End **********/
}

bool LineClipLiangBarsky(MyRect rect, Point& node1, Point& node2)
{
	// 请在此添加你的代码
   /********** Begin ********/

    float u1 = 0.0, u2 = 1.0;
    int dx = node2.x - node1.x;
    int dy = node2.y - node1.y;

    if (ClipTest(-dx, node1.x - rect.xmin, &u1, &u2)) {
        if (ClipTest(dx, rect.xmax - node1.x, &u1, &u2)) {
            if (ClipTest(-dy, node1.y - rect.ymin, &u1, &u2)) {
                if (ClipTest(dy, rect.ymax - node1.y, &u1, &u2)) {
                    if (u2 < 1.0) {
                        node2.x = node1.x + u2 * dx;
                        node2.y = node1.y + u2 * dy;
                    }
                    if (u1 > 0.0) {
                        node1.x = node1.x + u1 * dx;
                        node1.y = node1.y + u1 * dy;
                    }
                    return true;
                }
            }
        }
    }


   /********** End **********/
   return false;
}

void MyDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectf(rect.xmin, rect.ymin, rect.xmax, rect.ymax);

	for(int i = 0; i < 5; i+=2){
        bool bAccept = LineClipLiangBarsky(rect, vPoint[i], vPoint[i+1]);
		if(bAccept)
            LineGL(vPoint[i], vPoint[i+1]);
	}
	
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
    cv::imwrite("../img_step2/test.jpg", img);
	return 0;
}
