//g++ -o program program.cpp libglfw3dll.a libglew32.dll.a -I include -lglew32 -lglfw3 -lgdi32 -lopengl32

// This code takes two matrices and does the Dot Multiplication for it

#include <GL/glew.h>		/* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdlib.h> 

#define _USE_MATH_DEFINES


using namespace std;

float angX = 0.0*3.14159 / 180.0;
float angY = 0.0*3.14159 / 180.0;
float angZ = 0.0*3.14159 / 180.0;

void multMatrices(GLfloat m1[], GLfloat m2[], GLfloat result[]) {
	int row = 0;	// current row, honestly uneeded. 
	int col = 0;	// current col. Iterations of 4, resets to 0
	int index = 15;	// index of result matrix

	while (row < 4) {						// iterate through rows
		for (int i = 0; i < 4; i++) {			// iterate through columns
			if (i == 0) {
				col = 0;
				index -= 15;
			}
			result[index] = (m1[row] * m2[col] + m1[row + 4] * m2[col + 1] + m1[row + 8] * m2[col + 2] + m1[row + 12] * m2[col + 3]);
			col += 4;
			index += 4;
		}
		row++;
	}
}

int main() {

	GLfloat rotX[] = { 1, 0, 0, 0, //first column
				0, cos(angX), sin(angX), 0, //second column
				0, -sin(angX), cos(angX), 0, //third column
				0, 0, 0, 1 }; //fourth column

	GLfloat rotY[] = { cos(angY), 0, -sin(angY), 0, //first column
					0, 1, 0, 0, //second column
					sin(angY), 0, cos(angY), 0, //third column
					0, 0, 0, 1 }; //fourth column

	GLfloat rotZ[] = { cos(angZ), sin(angZ), 0, 0, //first column
					-sin(angZ), cos(angZ), 0, 0, //second column
					0, 0, 1, 0, //third column
					0, 0, 0, 1 }; //fourth column

	GLfloat m1[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
	GLfloat m2[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

	GLfloat results[16];
	multMatrices(m1, m2, results);
	for (int i = 0; i < 16; i++) {
		cout << results[i];
		cout << " ";
		if (i == 3 || i == 7 || i == 11)
		cout << "\n";
	}


	GLfloat results1[16];
	multMatrices(rotZ, results, results1);
	//cout << results1;

}