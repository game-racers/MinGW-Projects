//g++ -o partb.exe partb.cpp libglfw3dll.a libglew32.dll.a -O2 –I include –lglew32 -lglfw3 –lgdi32 –lopengl32

#include <GL/glew.h>	/* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <stdio.h>
#include <iostream>
#include "CImg/CImg.h"

using namespace cimg_library;
using namespace std;

int main() {

	CImg<unsigned char> smoothTriangle(500, 500, 1, 3, 0);

	//Vertices
	smoothTriangle(100, 100, 0, 0) = 255;		//Red
	smoothTriangle(100, 100, 0, 1) = 0;
	smoothTriangle(100, 100, 0, 2) = 0;
	smoothTriangle(400, 150, 0, 1) = 255;		//Green
	smoothTriangle(400, 150, 0, 0) = 0;
	smoothTriangle(400, 150, 0, 2) = 0;
	smoothTriangle(200, 300, 0, 2) = 255;		//Blue
	smoothTriangle(200, 300, 0, 0) = 0;
	smoothTriangle(200, 300, 0, 1) = 0;

	//Form the Triangle of Triangleness
		// Red Point to Green
	int dy = 50;
	int dx = 300;
	int d = dy - (dx / 2);
	int x = 100;
	int y = 100;

	while (x < 400)
	{
		x = x + 1;
		if (d < 0)
			d = d + dy;
		else
		{
			d = d + dy - dx;
			y = y + 1;
		}
		//R: 100,100	G: 400,150	B: 200,300
		smoothTriangle(x, y, 0, 0) = 255.0 * ((550.0 - x - y) / 350.0);		//Red ((300-x+50-y)/350)*255
		smoothTriangle(x, y, 0, 1) = ((x + y - 200.0) / 350) * 255.0;			//Green ((x+y-100)/350)*
	}
		// Blue Point to Green
	dy = 150;
	dx = 400 - 200;
	d = dy - (dx / 2);
	x = 200;
	y = 300;

	while (x < 400)
	{
		x = x + 1;
		if (d < 0)
			d = d + dy;
		else
		{
			d = d + dy - dx;
			y = y - 1;
		}
		smoothTriangle(x, y, 0, 1) = 255.0 * ((x + y - 500.0) / 50.0);			//Green
		smoothTriangle(x, y, 0, 2) = 255.0 * ((750.0-x-y)/50.0);			//Blue
	}
		// Red Point to Blue
	dy = 200;
	dx = 100;
	d = dx - (dy / 2);
	x = 100;
	y = 100;
	while (y < 300)
	{
		y = y + 1;
		if (d < 0)
			d = d + dx;
		else
		{
			d = d + dx - dy;
			x = x + 1;
		}
		smoothTriangle(x, y, 0, 0) = 255.0 * ((500.0 - x - y) / 300.0);		//Red
		smoothTriangle(x, y, 0, 2) = ((x + y - 200.0) / 300) * 255.0;			//Blu
	}
	
	//Fill it in!
	float totalArea = ((100.0 * (300 - 150) + 200.0 * (150 - 100) + 400.0 * (100 - 300)) / 2.0);
	int counter = 0;
	int fillTri = 0;
	for (int i = 101; i < 400; i++) {
		for (int j = 101; j < 300; j++) {
			if (smoothTriangle(i, j, 0, 0) != 0 || smoothTriangle(i, j, 0, 1) != 0 || smoothTriangle(i, j, 0, 2) != 0) {
				if (fillTri == 0)
					fillTri = 1;
				else
					break;
			}
			//R: 100,100	G: 400,150	B: 200,300
			if (fillTri == 1) {
				smoothTriangle(i, j, 0, 0) = (255.0 * (((float)(i) * (300 - 150) + 200.0 * (150 - j) + 400.0 * (j - 300)) / 2.0) / totalArea);
				smoothTriangle(i, j, 0, 1) = (255.0 * (((float)(i) * (100 - 300) + 100.0 * (300 - j) + 200.0 * (j - 100)) / 2.0) / totalArea);
				smoothTriangle(i, j, 0, 2) = (255.0 * (((float)(i) * (150 - 100) + 400.0 * (100 - j) + 100.0 * (j - 150)) / 2.0) / totalArea);
			}
		}
		fillTri = 0;
	}

	CImgDisplay disp2(smoothTriangle);
	while (!disp2.is_closed())
		disp2.wait();

	smoothTriangle.save("SmoothColoring-Triangle.bmp");
}