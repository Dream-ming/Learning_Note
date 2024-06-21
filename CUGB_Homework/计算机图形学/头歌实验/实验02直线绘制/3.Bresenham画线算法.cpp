#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // Please add the code here
    /********** Begin ********/
    int dx, dy, y, d;
    dx = x1 - x0;
    dy = y1 - y0;
    y = y0;
    d = -dx;

    for (int x = x0; x <= x1; ++x) {
        image.set(x, y, color);
        d += 2 * dy;
        if (d >= 0) {
            ++y;
            d = d - 2 * dx;
        }
    }

    /********** End *********/
}

int main(int argc, char** argv)
{
	TGAImage image(640,480, TGAImage::RGB);
    // Please add the code here
    /********** Begin ********/
	line(20, 20, 180, 140, image, white);
    /********** End *********/
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("../img_step2/test.tga");
	return 0;
}
