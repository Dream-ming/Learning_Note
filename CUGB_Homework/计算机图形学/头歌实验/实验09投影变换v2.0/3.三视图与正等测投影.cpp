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

Matrix projection(Vec3f eye, Vec3f center)
{
	Matrix m = Matrix::identity(4);
	m[3][2] = -1.f / (eye - center).norm();
	return m;
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

Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
	Vec3f z = (eye - center).normalize();
	Vec3f x = (up^z).normalize();
	Vec3f y = (z^x).normalize();
	Matrix res = Matrix::identity(4);
	for (int i = 0; i < 3; i++) {
		res[0][i] = x[i];
		res[1][i] = y[i];
		res[2][i] = z[i];
		res[i][3] = -center[i];
	}
	return res;
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
	const int width = 800;
	const int height = 800;
	const int depth = 255;


	//generate some image
	PNGImage image(width, height, PNGImage::RGBA); //Error when RGB because lodepng_get_raw_size_lct(w, h, colortype, bitdepth) > in.size() in encode
	image.init(black);
	model = new Model("cube.obj");
	Matrix ViewPort = viewport(width / 4, width / 4, width / 2, height / 2, depth);

	for (int i = 0; i < model->nfaces(); i++)
	{
		std::vector<int> face = model->face(i);
		for (int j = 0; j < (int)face.size(); j++)
		{
			Vec3f wp0 = model->vert(face[j]);
			Vec3f wp1 = model->vert(face[(j + 1) % face.size()]);

			Matrix S0 = scale(0.5, 0.4, 0.3);
			Vec3f swp0 = S0 * wp0;
			Vec3f swp1 = S0 * wp1;
			// Please add the code here
            /********** Begin ********/
			

            Matrix ProjectionX = Matrix::identity(4);
            ProjectionX[0][0] = 0.0f;
            Matrix ProjectionY = Matrix::identity(4);
            ProjectionY[1][1] = 0.0f;
            Matrix ProjectionZ = Matrix::identity(4);
            ProjectionZ[2][2] = 0.0f;
            
            Vec3f zp0 = ViewPort * ProjectionZ * swp0;
            Vec3f zp1 = ViewPort * ProjectionZ * swp1;
            line(zp0, zp1, image, white);
            
            Matrix rx = rotation_x(90);
            Matrix ty = translation(Vec3f(0, -1.2f, 0));
            Vec3f yp0 = ViewPort * ty * rx * ProjectionY * swp0;
            Vec3f yp1 = ViewPort * ty * rx * ProjectionY * swp1;
            line(yp0, yp1, image, red);
            
            Matrix ry = rotation_y(-90);
            Matrix tx = translation(Vec3f(-1.2f, 0, 0));
            Vec3f xp0 = ViewPort * tx * ry * ProjectionX * swp0;
            Vec3f xp1 = ViewPort * tx * ry * ProjectionX * swp1;
            line(xp0, xp1, image, green);
            
            Matrix rz = rotation_z(45);
            Matrix rx1 = rotation_x(35.25);
            Matrix ty1 = translation(Vec3f(0, 1.2f, 0));
            Vec3f rp0 = ViewPort * ty1 * ProjectionZ * rx1 * rz *swp0;
            Vec3f rp1 = ViewPort * ty1 * ProjectionZ * rx1 * rz *swp1;
            line(rp0, rp1, image, yellow);
			
            /********** End ********/
		}
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_png_file("../img_step3/test.png");
	delete model;
	return 0;
}
