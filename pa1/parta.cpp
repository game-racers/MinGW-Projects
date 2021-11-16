//g++ -o parta.exe parta.cpp libglfw3dll.a libglew32.dll.a -O2 –I include –lglew32 -lglfw3 –lgdi32 –lopengl32

#include <GL/glew.h>	/* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <stdio.h>
#include <iostream>
#include "CImg/CImg.h"

using namespace cimg_library;
using namespace std;

int main() {
	
	CImg<unsigned char> smoothSquare(500, 500, 1, 3, 0);

	// Corners First!
	smoothSquare(0, 0, 0, 0) = 255;		// Top Left
	smoothSquare(0, 0, 0, 1) = 0;
	smoothSquare(0, 0, 0, 2) = 0;
	smoothSquare(499, 0, 0, 1) = 255;		// Top Right
	smoothSquare(499, 0, 0, 0) = 0;
	smoothSquare(499, 0, 0, 2) = 0;
	smoothSquare(0, 499, 0, 2) = 255;		//Bottom Left
	smoothSquare(0, 499, 0, 0) = 0;
	smoothSquare(0, 499, 0, 1) = 0;
	smoothSquare(499, 499, 0, 0) = 255;	//Bottom Right
	smoothSquare(499, 499, 0, 1) = 255;
	smoothSquare(499, 499, 0, 2) = 255;

	// ----------------------------------------------- Line Next
	for (int i = 1; i < 499; i++) {
		smoothSquare(i, 0, 0, 0) = (smoothSquare(0, 0, 0, 0) * ((float)(499 - (float)i) / (float)499));
		//cout << (smoothSquare(0, 0, 0, 0) * ((float)(300 - i) / (float)300)) << endl;
		smoothSquare(i, 0, 0, 1) = (smoothSquare(499, 0, 0, 1) * ((float)i / (float)499));
		smoothSquare(i, 0, 0, 2) = 0;

		// Bottom
		smoothSquare(i, 499, 0, 0) = smoothSquare(0, 499, 0, 0) * ((float)(499 - i) / (float)499) + smoothSquare(499, 499, 0, 0) * ((float)(i) / (float)499);
		smoothSquare(i, 499, 0, 1) = smoothSquare(499, 499, 0, 1) * ((float)(i) / (float)499);
		smoothSquare(i, 499, 0, 2) = smoothSquare(499, 499, 0, 2);
	}

	// Construct the Square
	for (int i = 0; i < 500; i++) {
		for (int j = 1; j < 499; j++) {
			smoothSquare(i, j, 0, 0) = (smoothSquare(i, 0, 0, 0) * ((float)(500 - j) / (float)500)) + (smoothSquare(i, 499, 0, 0) * ((float)(j) / (float)500)); //R
			smoothSquare(i, j, 0, 1) = (smoothSquare(i, 0, 0, 1) * ((float)(500 - j) / (float)500)) + (smoothSquare(i, 499, 0, 1) * ((float)(j) / (float)500)); //G
			smoothSquare(i, j, 0, 2) = (smoothSquare(i, 0, 0, 2) * ((float)(500 - j) / (float)500)) + (smoothSquare(i, 499, 0, 2) * ((float)(j) / (float)500)); //B
		}
	}

	//Display and save filtered image
	CImgDisplay disp1(smoothSquare);
	while (!disp1.is_closed())
		disp1.wait();

	smoothSquare.save("SmoothColoring-Square.bmp");
}