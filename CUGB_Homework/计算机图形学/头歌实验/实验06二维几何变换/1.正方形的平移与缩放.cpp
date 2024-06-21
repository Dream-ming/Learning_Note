// 提示：写完代码请保存之后再进行评测
#include <GL/freeglut.h>
#include<stdio.h>

// 评测代码所用头文件-开始
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
// 评测代码所用头文件-结束

void init(void)
{  
    glClearColor(0.0, 0.0, 0.0, 0.0);       //设置背景颜色
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-5.0, 5.0, -5.0, 5.0);       //设置显示的范围是X:-5.0~5.0, Y:-5.0~5.0
	glMatrixMode(GL_MODELVIEW);
}

void myDraw(void)                           //二维几何变换
{
   // 请在此添加你的代码
   /********** Begin ********/

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glRectf(-1.0, -1.0, 1.0, 1.0);
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(3.0, 0.5, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glRectf(-1.0, -1.0, 1.0, 1.0);
    glPopMatrix();


   /********** End **********/  						
	glFlush();
}

int main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("几何变换示例");
    
    init();
	glutDisplayFunc(&myDraw);
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
        //cv::waitKey();
        cv::imwrite("../img_step1/test.jpg", img);
	return 0;
}
