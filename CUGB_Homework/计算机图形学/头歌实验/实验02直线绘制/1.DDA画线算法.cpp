#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // Please add the code here
    /********** Begin ********/

    float y, k;
    k = (y1 - y0) * 1.0 / (x1 - x0);
    y = y0;
    for (int x = x0; x <= x1; ++x) {
        image.set(x, (int)(y + 0.5), color);
        y += k;
    }

    /********** End *********/
}

int main(int argc, char** argv)
{
	TGAImage image(640,480, TGAImage::RGB);
	line(13, 20, 180, 140, image, white);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("../img_step1/test.tga");

	return 0;
}
