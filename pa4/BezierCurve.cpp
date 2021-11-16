//g++ -o BezierCurve.exe BezierCurve.cpp -O2 -lgdi32

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
	unsigned char color1[] = { 138, 43, 226 };
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
	for (int i = 0; i < coords_x.size(); i++) {
		drawing.draw_circle(coords_x[i], coords_y[i], 5, color1);
	}

	vector<int> linX;	// linear interpolation
	vector<int> linY;
	vector<int> quadX;	// quadratic interpolation
	vector<int> quadY;
	vector<int> tempX;	// temp to clear out quadratic and restore it. 
	vector<int> tempY;
	int ferrari[] = { 255, 40, 0 };
	int count = 0;

	for (float step = 0; step <= 1.0; step += 0.01) {		// for each 1.0 is 100 steps. 
		if (coords_x.size() > 2) {
			for (int line = 0; line < coords_x.size() - 1; line++) {		// Linear Interpolation of each line. 
				linX.push_back((1 - step) * coords_x[line] + step * coords_x[line + 1]);
				linY.push_back((1 - step) * coords_y[line] + step * coords_y[line + 1]);
			}
			
			for (int quad = 0; quad < linX.size() - 1; quad++) {			//  Linear Interpolation of each of the line of lines
				quadX.push_back((1 - step) * linX[quad] + step * linX[quad + 1]);
				quadY.push_back((1 - step) * linY[quad] + step * linY[quad + 1]);
			}
			
			while (quadX.size() >= 2) {
				tempX.clear();
				tempY.clear();
				tempX.swap(quadX);	// clear out the quad values
				tempY.swap(quadY);

				for (int cube = 0; cube < tempX.size() - 1; cube++) {		// Linear Interpolation of each line of line of lines. 
					quadX.push_back((1 - step) * tempX[cube] + step * tempX[cube + 1]);
					quadY.push_back((1 - step) * tempY[cube] + step * tempY[cube + 1]);
					
				}
			}
			
			drawing.draw_circle(quadX[0], quadY[0], 3, ferrari);
			linX.clear();
			linY.clear();
			quadX.clear();
			quadY.clear();
		}
		else 
			drawing.draw_circle(coords_x[count], coords_y[count], 3, ferrari);
		count++;
	}
	cout << "test end" << endl;
	CImgDisplay disp1(drawing);
	while (!disp1.is_closed())
		disp1.wait();
}
