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

Matrix translation(Vec3f v) {
	Matrix Tr = Matrix::identity(4);
	Tr[0][3] = v.x;
	Tr[1][3] = v.y;
	Tr[2][3] = v.z;
	return Tr;
}

Matrix scale(float factorX, float factorY, float factorZ)
{
	Matrix Z = Matrix::identity(4);
	Z[0][0] = factorX;
	Z[1][1] = factorY;
	Z[2][2] = factorZ;
	return Z;
}

Matrix rotation_x(float angle)
{
	angle = angle * PI / 180;
	float sinangle = sin(angle);
	float cosangle = cos(angle);

	Matrix R = Matrix::identity(4);
	R[1][1] = R[2][2] = cosangle;
	R[1][2] = -sinangle;
	R[2][1] = sinangle;
	return R;
}

Matrix rotation_y(float angle)
{
	angle = angle * PI / 180;
	float sinangle = sin(angle);
	float cosangle = cos(angle);

	Matrix R = Matrix::identity(4);
	R[0][0] = R[2][2] = cosangle;
	R[0][2] = sinangle;
	R[2][0] = -sinangle;
	return R;
}

Matrix rotation_z(float angle) {
	angle = angle * PI / 180;
	float sinangle = sin(angle);
	float cosangle = cos(angle);

	Matrix R = Matrix::identity(4);
	R[0][0] = R[1][1] = cosangle;
	R[0][1] = -sinangle;
	R[1][0] = sinangle;
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
	Vec3f eye(0.5, 1.5, 4);
	Vec3f center(0, 0, 0);

	Matrix Projection = Matrix::projection(eye, center);
	Matrix ViewPort = Matrix::viewport(width / 4, width / 4, width / 2, height / 2, depth);


	//generate some image
	PNGImage image(width, height, PNGImage::RGBA); //Error when RGB because lodepng_get_raw_size_lct(w, h, colortype, bitdepth) > in.size() in encode
	image.init(black);
	model = new Model("cube.obj");


	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> face = model->face(i);
		for (int j = 0; j < (int)face.size(); j++)
		{
			Vec3f wp0 = model->vert(face[j]);
			Vec3f wp1 = model->vert(face[(j + 1) % face.size()]);

			Matrix S0 = scale(0.5, 0.5, 0.5);
			Vec3f swp0 = S0 * wp0;
			Vec3f swp1 = S0 * wp1;

			// for comparing - draw the model after scaled 
			Vec3f op0 = ViewPort * Projection * swp0;
			Vec3f op1 = ViewPort * Projection * swp1;
			line(op0, op1, image, white);

			// Please add the code here
            /********** Begin ********/

            Matrix S1 = translation(Vec3f(1.2, 0.0, 0.0));
            op0 = ViewPort * Projection * S1 * swp0;
            op1 = ViewPort * Projection * S1 * swp1;
            line(op0, op1, image, red);

            Matrix S3 = rotation_y(30.0);
            Matrix S4 = translation(Vec3f(-1.3, 0.0, 0.0));
            op0 = ViewPort * Projection * S4 * S3 * swp0;
            op1 = ViewPort * Projection * S4 * S3 * swp1;
            op0 = ViewPort * Projection * S4 * S3 * swp0;
            op1 = ViewPort * Projection * S4 * S3 * swp1;
            line(op0, op1, image, green);

            /********** End *********/
		}
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_png_file("../img_step1/test.png");
	delete model;
	return 0;
}
