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

Vec3f barycentric(Vec3f *pts, Vec3f P)
{
	Vec3f u = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x) ^
		Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y);
	/* `pts` and `P` has integer value as coordinates
	   so `abs(u[2])` < 1 means `u[2]` is 0, that means
	   triangle is degenerate, in this case return something with negative coordinates */
	if (std::abs(u.z) < 1)
		return Vec3f(-1, 1, 1);
	return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void triangle(Vec3f *pts, float *zbuffer, PNGImage &image, PNGColor color)
{
	int minX = min({ pts[0].x, pts[1].x, pts[2].x });
	int maxX = max({ pts[0].x, pts[1].x, pts[2].x });
	int minY = min({ pts[0].y, pts[1].y, pts[2].y });
	int maxY = max({ pts[0].y, pts[1].y, pts[2].y });

	int width = image.get_width();
	int height = image.get_height();
	if (maxX >= width)
		maxX = width - 1;
	if (maxY >= height)
		maxY = height - 1;

	Vec3f P;
	for (P.x = minX; P.x <= maxX; P.x++) {
		for (P.y = minY; P.y <= maxY; P.y++) {
			Vec3f bc_screen = barycentric(pts, P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = 0;
			for (int i = 0; i < 3; i++)
				P.z += pts[i].z * bc_screen[i];
			if (zbuffer[int(P.x + P.y*width)] < P.z) {
				zbuffer[int(P.x + P.y*width)] = P.z;
				image.set(P.x, P.y, color);
			}
		}
	}
}

void triangle(Vec3f *pts, float *zbuffer, PNGImage &image, Vec3f intensity)
{
	int minX = min({ pts[0].x, pts[1].x, pts[2].x });
	int maxX = max({ pts[0].x, pts[1].x, pts[2].x });
	int minY = min({ pts[0].y, pts[1].y, pts[2].y });
	int maxY = max({ pts[0].y, pts[1].y, pts[2].y });

	int width = image.get_width();
	int height = image.get_height();
	if (maxX >= width)
		maxX = width - 1;
	if (maxY >= height)
		maxY = height - 1;

	Vec3f P;
	for (P.x = minX; P.x <= maxX; P.x++) {
		for (P.y = minY; P.y <= maxY; P.y++) {
			Vec3f bc_screen = barycentric(pts, P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = 0;
			for (int i = 0; i < 3; i++)
				P.z += pts[i].z * bc_screen[i];
			if (zbuffer[int(P.x + P.y*width)] < P.z) {
				zbuffer[int(P.x + P.y*width)] = P.z;
                //三角形内部点的光强根据重心坐标与顶点光强插值来计算
				/********** Begin ********/
				float intensity1 = intensity * bc_screen ;
				PNGColor clr = PNGColor(255 * intensity1, 255 * intensity1, 255 * intensity1, 255)  ;
				/********** End ********/
				image.set(P.x, P.y, clr);
			}
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
	const PNGColor orange = PNGColor(255, 128, 0, 255);
	const PNGColor clr[6] = { green, yellow, blue, red, white, orange };

	Model *model = NULL;
	const int width = 800;
	const int height = 800;
	const int depth = 255;

	//generate some image
	PNGImage image(width, height, PNGImage::RGBA); //Error when RGB because lodepng_get_raw_size_lct(w, h, colortype, bitdepth) > in.size() in encode
	image.init(black);
	model = new Model("african_head.obj");

	float *zbuffer = new float[width*height];
	for (int i = 0; i < width * height; i++)
		zbuffer[i] = -numeric_limits<float>::max();

	Vec3f eye(0, 0, 4), center(0, 0, 0), up(0, 1, 0);
	Vec3f light_dir(0, 0, 1);
	Matrix ModelView = lookat(eye, center, up);
	Matrix Projection = projection(eye, center);
	Matrix ViewPort = viewport(width / 4, width / 4, width / 2, height / 2, depth);
	Matrix VPM = ViewPort * Projection * ModelView;

	for (int i = 0; i < model->nfaces(); i++)
	{
		vector<int> face = model->face(i);
		Vec3f world_coords[3], screen_coords[3], vert_normal[3];
		Vec3f intensity;
		for (int j = 0; j < 3; j++)
		{
			world_coords[j] = model->vert(face[j]);
			screen_coords[j] = VPM * world_coords[j];
			vert_normal[j] = model->normal(i, j);
			//三角形每个顶点的光强根据顶点法线向量与光照方向的点积来计算
			/********** Begin ********/
			intensity[j] = max(0.0f, vert_normal[j] * light_dir)  ;
			/*********** End *********/
		}

		triangle(screen_coords, zbuffer, image, intensity);
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_png_file("../img_step1/test.png");
	delete model;
	delete zbuffer;
	return 0;
}
