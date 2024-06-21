#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // Please add the code here
    /********** Begin ********/

    int a, b, d1, d2, d, x, y;

    a = y0 - y1;
    b = x1 - x0;
    d = 2 * a + b;
    d1 = 2 * a;
    d2 = 2 * (b + a);
    x = x0;
    y = y0;

    image.set(x, y, color);
    while (x < x1) {
        if (d <= 0) {
            ++x;
            ++y;
            d += d2;
        }
        else {
            ++x;
            d += d1;
        }
        image.set(x, y, color);
    }

    /********** End *********/
}

int main(int argc, char** argv)
{
	TGAImage image(640,480, TGAImage::RGB);
    // Please add the code here
    /********** Begin ********/
	line(100, 100, 520, 300, image, red);
    /********** End *********/
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("../img_step4/test.tga");
	return 0;
}
