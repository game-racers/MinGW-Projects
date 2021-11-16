//g++ -o ChaikinCurve.exe ChaikinCurve.cpp -O2 -lgdi32

#include <iostream>
#include "CImg/CImg.h"
#include <vector>

using namespace cimg_library;
using namespace std;

int main() {
	CImg<unsigned char> user(500, 500, 1, 3, 0);
		//Use this block of code as a starting point for letting the user click points on a window
		//This will draw a circle at the selected point and print the coordinate
		//You'll need to add-on to save the coordinates

	vector<int> coords_x;
	vector<int> coords_y;

	CImgDisplay window(user, "Click control points");
	unsigned char color[] = { 255,128,64 };
	unsigned char color1[] = { 255, 0, 0 };
	while (!window.is_closed()) {
		window.wait();
		if (window.button()) {
			int x = window.mouse_x();
			int y = window.mouse_y();
			cout << x << ", " << y << endl;
			coords_x.push_back(x);
			coords_y.push_back(y);
			user.draw_circle(x, y, 5, color);
			window.display(user);
		}
	}

	CImg<unsigned char> drawing(500, 500, 1, 3, 0);		// contains line segments to the original input points only
		//For Chaikin, you'll want to draw line segments connecting points and for Bezier you'll want to draw circles
	unsigned char dots[] = { 255,0,255 };
	for (int i = 0; i < coords_x.size() - 1; i++) {
		drawing.draw_line(coords_x[i], coords_y[i], coords_x[i+1], coords_y[i+1], dots);
		drawing.draw_circle(coords_x[i], coords_y[i], 5, color1);
	}
	drawing.draw_circle(coords_x[coords_x.size()-1], coords_y[coords_y.size()-1], 5, color1);

	CImgDisplay disp1(drawing);		// Initial lines, iteration 0
	while (!disp1.is_closed())
		disp1.wait();

	vector<int> newPointsX;
	vector<int> newPointsY;
	int colour[] = { 63, 63, 63 };		// starts at 63 so 32 can be added safely 6 times over maxing at 255. 
	vector<int> pointsX;
	vector<int> pointsY;
	int tempX;
	int tempY;

	for (int z = 0; z < 6; z++) {
		cout << "Iteration: " << z + 1 << endl;
		for (int j = 0; j < coords_x.size() - 1; j++) {		//get the midpoint and the p and r values
			tempX = (coords_x[j] + coords_x[j + 1]) / 2;		//midpoint values
			tempY = (coords_y[j] + coords_y[j + 1]) / 2;

			pointsX.push_back((coords_x[j] + tempX) / 2);	// point p 25%
			pointsY.push_back((coords_y[j] + tempY) / 2);
			pointsX.push_back((tempX + coords_x[j + 1]) / 2);	// point r 75%
			pointsY.push_back((tempY + coords_y[j + 1]) / 2);
		}
		newPointsX.swap(pointsX);	//swaos out the points and clears out the temp vectors. 
		newPointsY.swap(pointsY);
		pointsX.clear();
		pointsY.clear();

		//draw(drawing, newPointsX, newPointsY, colour);
		for (int i = 0; i < newPointsX.size() - 1; i++)		// iterate through newPoints and draw them
			drawing.draw_line(newPointsX[i], newPointsY[i], newPointsX[i + 1], newPointsY[i + 1], colour);
		
		disp1 = drawing;	// update the image and display it. 
		disp1.show();
		while (!disp1.is_closed())
			disp1.wait();

		coords_x.swap(newPointsX);		// swap points, clear out newPoints and change the color to a lighter shade of grey. 
		coords_y.swap(newPointsY);
		newPointsX.clear();
		newPointsY.clear();
		colour[0] += 32;
		colour[1] += 32;
		colour[2] += 32;
	}
}