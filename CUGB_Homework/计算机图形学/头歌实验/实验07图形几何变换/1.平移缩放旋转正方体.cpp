#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "model.h"
#include "geometry.h"
#include "pngimage.h"

using namespace std;
const double PI = acos(-1.0);

void line(Vec3i p0, Vec3i p1, PNGImage  &image, PNGColor color)
{
	bool steep = false;
	if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y))
	{
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
		steep = true;
	}
	if (p0.x > p1.x)
	{
		std::swap(p0.x, p1.x);
		std::swap(p0.y, p1.y);
	}

	int dx = p1.x - p0.x;
	int dy = std::abs(p1.y - p0.y);

	int y = p0.y;
	int d = -dx;
	for (int x = p0.x; x <= p1.x; x++)
	{
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);

		d = d + 2 * dy;
		if (d > 0)
		{
			y += (p1.y > p0.y ? 1 : -1);
			d = d - 2 * dx;
		}
	}
}

Matrix viewport(int x, int y, int w, int h, int depth) {
	Matrix m = Matrix::identity(4);
	m[0][3] = x + w / 2.f;
	m[1][3] = y + h / 2.f;
	m[2][3] = depth / 2.f;

	m[0][0] = w / 2.f;
	m[1][1] = h / 2.f;
	m[2][2] = depth / 2.f;
	return m;
}

Matrix translation(Vec3f v) {
	Matrix Tr = Matrix::identity(4);
    // Please add the code here
    /********** Begin ********/

    Tr[0][3] = v[0];
    Tr[1][3] = v[1];
    Tr[2][3] = v[2];

    /********** End *********/
	return Tr;
}

Matrix scale(float factorX, float factorY, float factorZ)
{
	Matrix Z = Matrix::identity(4);
    /********** Begin ********/

    Z[0][0] = factorX;
    Z[1][1] = factorY;
    Z[2][2] = factorZ;

    /********** End *********/
	return Z;
}

Matrix rotation_x(float angle)
{
	angle = angle * PI / 180;
	float sinangle = sin(angle);
	float cosangle = cos(angle);
	Matrix R = Matrix::identity(4);
    /********** Begin ********/

    R[1][1] = cosangle;
    R[1][2] = -sinangle;
    R[2][1] = sinangle;
    R[2][2] = cosangle;

    /********** End *********/
	return R;
}

Matrix rotation_y(float angle)
{
	angle = angle * PI / 180;
	float sinangle = sin(angle);
	float cosangle = cos(angle);

	Matrix R = Matrix::identity(4);
    /********** Begin ********/

    R[0][0] = cosangle;
    R[0][2] = sinangle;
    R[2][0] = -sinangle;
    R[2][2] = cosangle;

    /********** End *********/
	return R;
}

Matrix rotation_z(float angle) {
	angle = angle * PI / 180;
	float sinangle = sin(angle);
	float cosangle = cos(angle);

	Matrix R = Matrix::identity(4);
    /********** Begin ********/

    R[0][0] = cosangle;
    R[0][1] = -sinangle;
    R[1][0] = sinangle;
    R[1][1] = cosangle;

    /********** End *********/
	return R;
}

int main(int argc, char** argv)
{
	const PNGColor white = PNGColor(255, 255, 255, 255);
	const PNGColor black = PNGColor(0, 0, 0, 255);
	const PNGColor red = PNGColor(255, 0, 0, 255);
	const PNGColor green = PNGColor(0, 255, 0, 255);
	const PNGColor blue = PNGColor(0, 0, 255, 255);
	const PNGColor yellow = PNGColor(255, 255, 0, 255);

	Model *model = NULL;
	const int width = 500;
	const int height = 500;
	const int depth = 255;

	//generate some image
	PNGImage image(width, height, PNGImage::RGBA); //Error when RGB because lodepng_get_raw_size_lct(w, h, colortype, bitdepth) > in.size() in encode
	image.init(black);
	model = new Model("../step3/cube.obj");
	Matrix VP = viewport(width / 4, width / 4, width / 2, height / 2, depth);

	{ // draw the axes
		Vec3f x(1.f, 0.f, 0.f), y(0.f, 1.f, 0.f), o(0.f, 0.f, 0.f);
		o = VP*o;
		x = VP*x;
		y = VP*y;
		line(o, x, image, red);
		line(o, y, image, green);
	}

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < (int)face.size(); j++) {
			Vec3f wp0 = model->vert(face[j]);
			Vec3f wp1 = model->vert(face[(j + 1) % face.size()]);
       
			// draw the original model
			Vec3f op0 = VP*wp0;
			Vec3f op1 = VP*wp1;
			line(op0, op1, image, white);

			// draw the translated model
            // Please add the code here
            /********** Begin ********/
			Matrix T = translation(Vec3f(0.5  ,  0.5 ,  0.5 ));
            /********** End *********/
			Vec3f tp0 = VP*T*wp0;
			Vec3f tp1 = VP*T*wp1;
			line(tp0, tp1, image, red);

			// draw the scaled model
            // Please add the code here
            /********** Begin ********/            
			Matrix S = scale( 0.5 , 0.5 , 0.5 );
            /********** End *********/
			Vec3f sp0 = VP*S*wp0;
			Vec3f sp1 = VP*S*wp1;
			line(sp0, sp1, image, green);

			// draw the rotated model
            // Please add the code here
            /********** Begin ********/   
			Matrix R = rotation_z( 45 );
            /********** End *********/
			Vec3f rp0 = VP*R*wp0;
			Vec3f rp1 = VP*R*wp1;
			line(rp0, rp1, image, yellow);
		}
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_png_file("../img_step3/test.png");
	delete model;
	return 0;
}
