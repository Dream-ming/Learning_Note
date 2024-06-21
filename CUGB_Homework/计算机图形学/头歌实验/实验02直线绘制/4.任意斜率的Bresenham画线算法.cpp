#include "tgaimage.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "model.h"
#include "geometry.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model *model = NULL;
const int width = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
    // Please add the code here
    /********** Begin ********/

    int flag = 0;
    if (abs(x0 - x1) < abs(y0 - y1)) {
        swap(x0, y0);
        swap(x1, y1);
        flag = 1;
    }
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int y = y0;
    int d = -dx;
    
    for (int x = x0; x <= x1; ++x) {
        if (flag) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
        d += 2 * dy;
        if (d >= 0) {
            y += (y1 > y0);
            y -= (y1 < y0);
            d = d - 2 * dx;
        }
    }

    /********** End *********/
}

int main(int argc, char** argv)
{
	model = new Model("african_head.obj");
	TGAImage image(width, height, TGAImage::RGB);
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.)*width / 2.;
			int y0 = (v0.y + 1.)*height / 2.;
			int x1 = (v1.x + 1.)*width / 2.;
			int y1 = (v1.y + 1.)*height / 2.;
			line(x0, y0, x1, y1, image, white);
		}
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("../img_step3/test.tga");
	delete model;
	return 0;
}
