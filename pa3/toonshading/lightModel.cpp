/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries separate legal notices                              |
|******************************************************************************|
| Phong Illumination                                                           |
| Apple: remember to uncomment version number hint in start_gl()               |
\******************************************************************************/
//g++ -o lightModel gl_utils.cpp math_funcs.cpp lightModel.cpp libglfw3dll.a libglew32.dll.a -I include -lglew32 -lglfw3 -lgdi32 -lopengl32

#include "math_funcs.h"
#include "gl_utils.h"
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define GL_LOG_FILE "gl.log"

using namespace std;

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow* g_window = NULL;


int countLabel(string modelName, char label[]){
	int numLab = 0;
	
	FILE *objFile;
	objFile = fopen(modelName.c_str(),"r");

	char buf[128];
	while (fscanf(objFile, "%s", &buf) != EOF){
	    if (strcmp(buf,label) == 0)
		numLab++;
	}
	
	cout << "Model has " << numLab << " " << label << "\n";
	fclose(objFile);
	return numLab;

}


void loadVertices(string modelName, GLfloat verts[]){	
	cout << "Loading vertices\n";
	int numVert = 0;

	FILE *objFile;
	objFile = fopen(modelName.c_str(),"r");

	float maxX = -100000000.0;
	float minX = 100000000.0;
	float maxY = -100000000.0;
	float minY = 100000000.0;
	float maxZ = -100000000.0;
	float minZ = 100000000.0;

	char buf[128];
	char label[] = "v";
	float a, b, c;
	while (fscanf(objFile, "%s", &buf) != EOF){ 
	    if (strcmp(buf,label) == 0){
		//cout << "vertex" << endl;
		fscanf(objFile, "%f %f %f\n", &a, &b, &c);
		//cout << "a,b,c: " << a << ", " << b << ", " << c << endl;
		if (a > maxX)
		    maxX = a;
		if (a < minX)
		    minX = a;
		if (b > maxY)
		    maxY = b;
		if (b < minY)
		    minY = b;
		if (c > maxZ)
		    maxZ = c;
		if (c < minZ)
		    minZ = c;
		verts[3*numVert + 0] = 1.0*a;
		verts[3*numVert + 1] = 1.0*b;
		verts[3*numVert + 2] = 1.0*c;
		numVert++;
	    }
	}

	float scaleX = maxX-minX;
	float scaleY = maxY-minY;
	float scaleZ = maxZ-minZ;
	float transX = 0.5*(maxX+minX);
	float transY = 0.5*(maxY+minY);
	float transZ = 0.5*(maxZ+minZ);
	cout << "scales: " << scaleX << ", " << scaleY << ", " << scaleZ << endl;
	cout << "trans: " << transX << ", " << transY << ", " << transZ << endl;

	for (int i = 0; i < numVert; i++){
	    verts[3*i+0] = (verts[3*i+0] - transX)/scaleX;
	    verts[3*i+1] = (verts[3*i+1] - transY)/scaleY;
	    verts[3*i+2] = (verts[3*i+2] - transZ)/scaleZ;
	}

	fclose(objFile);
	cout << "Done loading vertices\n";
}




void loadFaces(string modelName, GLint faces[]){	
	cout << "Loading faces\n";
	int numFaces = 0;

	FILE *objFile;
	objFile = fopen(modelName.c_str(),"r");

	char buf[128];
	char label[] = "f";
	GLfloat a, b, c, x, y, z;
	while (fscanf(objFile, "%s", &buf) != EOF){ 
	    if (strcmp(buf,label) == 0){
		//fscanf(objFile, "%f\\%f %f\\%f %f\\%f\n", &a, &x, &b, &y, &c, &z);
		fscanf(objFile, "%f %f %f\n", &a, &b, &c);
		faces[3*numFaces + 0] = a-1;
		faces[3*numFaces + 1] = b-1;
		faces[3*numFaces + 2] = c-1;
		numFaces++;
	    }
	}

	fclose(objFile);
	cout << "Done loading faces\n";
}


void computeFaceNormals(GLfloat faceNormals[], GLfloat verts[], GLint faces[], int numFaces){
	for (int i = 0; i < numFaces; i++){
		int idx1 = faces[i*3 + 0];
		int idx2 = faces[i*3 + 1];
		int idx3 = faces[i*3 + 2];
		float ux = verts[idx1*3 + 0] - verts[idx2*3 + 0];
		float uy = verts[idx1*3 + 1] - verts[idx2*3 + 1];
		float uz = verts[idx1*3 + 2] - verts[idx2*3 + 2];
		float vx = verts[idx1*3 + 0] - verts[idx3*3 + 0];
		float vy = verts[idx1*3 + 1] - verts[idx3*3 + 1];
		float vz = verts[idx1*3 + 2] - verts[idx3*3 + 2];
		float nx = uy*vz - uz*vy;
		float ny = uz*vx - ux*vz;
		float nz = ux*vy - uy*vx;
		float mag = sqrt(nx*nx + ny*ny + nz*nz);
		
	//	cout << "avg norm" << nx << ", " << ny << ", " << nz << endl;
		//cout << "mag: " << mag << endl;
		faceNormals[3*i + 0] = nx/mag;
		faceNormals[3*i + 1] = ny/mag;
		faceNormals[3*i + 2] = nz/mag;
	}
	
}



void computeVertNormals(GLfloat normals[], GLfloat verts[], int numVerts, GLint faces[], int numFaces, GLfloat faceNormals[]){
	for (int i = 0; i < numVerts; i++){
		float avgX = 0.0;
		float avgY = 0.0;
		float avgZ = 0.0;
		int numF_vert = 0;
		
		//find all the faces that contain this vertex
		for (int j = 0; j < numFaces; j++){
			int found = 0;
			for (int k = 0; k < 3; k++)
				if (faces[j*3 + k] == i)
					found = 1;
			if (found){
				avgX += faceNormals[j*3 + 0];
				avgY += faceNormals[j*3 + 1];
				avgZ += faceNormals[j*3 + 2];				
				numF_vert++;				
			}			
		}
		
		avgX /= numF_vert;
		avgY /= numF_vert;
		avgZ /= numF_vert;
		//cout << "avg norm" << avgX << ", " << avgY << ", " << avgZ << endl;
		normals[i*3 + 0] = avgX;
		normals[i*3 + 1] = avgY;
		normals[i*3 + 2] = avgZ;
	}
}


int main () {
	
	//Read in obj
	string modelName = "objectFiles/bunny.obj";

	int numVert = countLabel(modelName, "v");
	GLfloat* verts = new GLfloat[3*numVert];
	loadVertices(modelName, verts);
	
	int numFaces = countLabel(modelName, "f");
	GLint* faces = new GLint[3*numFaces];
	loadFaces(modelName, faces);
	
	
	GLfloat* faceNormals = new GLfloat[3*numFaces];
	computeFaceNormals(faceNormals, verts, faces, numFaces);
	
	GLfloat* vertNormals = new GLfloat[3*numVert];
	computeVertNormals(vertNormals, verts, numVert, faces, numFaces, faceNormals);


	GLfloat* points = new GLfloat[9*numFaces];
	GLfloat* normals = new GLfloat[9*numFaces];
	for (int i = 0; i < numFaces; i++){
	    int idx1 = faces[3*i + 0];
	    int idx2 = faces[3*i + 1];
	    int idx3 = faces[3*i + 2];
	    points[i*9 + 0] = verts[3*idx1+0];
	    points[i*9 + 1] = verts[3*idx1+1];
	    points[i*9 + 2] = verts[3*idx1+2];
	    points[i*9 + 3] = verts[3*idx2+0];
	    points[i*9 + 4] = verts[3*idx2+1];
	    points[i*9 + 5] = verts[3*idx2+2];
	    points[i*9 + 6] = verts[3*idx3+0];
	    points[i*9 + 7] = verts[3*idx3+1];
	    points[i*9 + 8] = verts[3*idx3+2];
	    normals[i*9 + 0] = vertNormals[3*idx1+0];
	    normals[i*9 + 1] = vertNormals[3*idx1+1];
	    normals[i*9 + 2] = vertNormals[3*idx1+2];
	    normals[i*9 + 3] = vertNormals[3*idx2+0];
	    normals[i*9 + 4] = vertNormals[3*idx2+1];
	    normals[i*9 + 5] = vertNormals[3*idx2+2];
	    normals[i*9 + 6] = vertNormals[3*idx3+0];
	    normals[i*9 + 7] = vertNormals[3*idx3+1];
	    normals[i*9 + 8] = vertNormals[3*idx3+2];
	}
	int numPoints = 3*numFaces;
	//cout << "Done loading points\n";
	
	
	
	restart_gl_log ();
	// start GL context and O/S window using the GLFW helper library
	start_gl ();
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

	/* OTHER STUFF GOES HERE NEXT */
	/*GLfloat points[] = {
		 100.0f,	100.5f,	-200.0f,
		 100.5f, -100.5f,	200.0f,
		-100.5f, -100.5f,	-200.0f
	};*/
	
	/*GLfloat points2[] = {
	  -1.5f, -0.5f,  1.0f,
	   1.5f, -0.5f,  1.0f,
	   0.0f,  0.5f,  1.0f
	};

	int numPoints2 = 3;
	
	GLfloat normals2[] = {
		1.0f, -1.0f,  -1.0f,
		1.0f, 0.0f,  -1.0f,
		1.0f, 1.0f,  -1.0f
	};*/
	
	GLuint points_vbo;
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * numPoints * sizeof (GLfloat), points, GL_STATIC_DRAW);
	
	GLuint normals_vbo;
	glGenBuffers (1, &normals_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * numPoints * sizeof (GLfloat), normals, GL_STATIC_DRAW);
	
	
	/*GLuint points_vbo2;
	glGenBuffers (1, &points_vbo2);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo2);
	glBufferData (GL_ARRAY_BUFFER, 3 * numPoints2 * sizeof (GLfloat), points2, GL_STATIC_DRAW);
	
	GLuint normals_vbo2;
	glGenBuffers (1, &normals_vbo2);
	glBindBuffer (GL_ARRAY_BUFFER, normals_vbo2);
	glBufferData (GL_ARRAY_BUFFER, 3 * numPoints2 * sizeof (GLfloat), normals2, GL_STATIC_DRAW);*/
	
	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	
	
	
	/*GLuint vao2;
	glGenVertexArrays (1, &vao2);
	glBindVertexArray (vao2);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo2);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, normals_vbo2);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);*/
	
	
	GLuint shader_programme = create_programme_from_files (
		"vertShader.glsl", "toonFragShader.glsl");
	
	#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
	// input variables
	float near = 0.1f; // clipping plane
	float far = 100.0f; // clipping plane
	float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
	float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
	// matrix components
	float range = tan (fov * 0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect + range * aspect);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);
	GLfloat proj_mat[] = {
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, -1.0f,
		0.0f, 0.0f, Pz, 0.0f
	};
	/*GLfloat proj_mat[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0	};*/
	
	/* create VIEW MATRIX */
	float cam_pos[] = {0.0f, 0.0f, 2.0f}; // don't start at zero, or we will be too close
	float cam_yaw = 0.0f; // y-rotation in degrees
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;
	
	/* matrix for moving the triangle */
	mat4 model_mat = identity_mat4 ();
	
	glUseProgram (shader_programme);
	int view_mat_location = glGetUniformLocation (shader_programme, "view_mat");
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	int proj_mat_location = glGetUniformLocation (shader_programme, "projection_mat");
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);
	int model_mat_location = glGetUniformLocation (shader_programme, "model_mat");
	glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, model_mat.m);
	
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	while (!glfwWindowShouldClose (g_window)) {
		_update_fps_counter (g_window);
		double current_seconds = glfwGetTime ();
		
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearColor(0.3, 0.3, 0.3, 1.0);
		glViewport (0, 0, g_gl_width, g_gl_height);
		
		glUseProgram (shader_programme);
		
		model_mat.m[12] = sinf (current_seconds);
		glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, model_mat.m);
		
		glBindVertexArray (vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays (GL_TRIANGLES, 0, numPoints);
		
		//glBindVertexArray (vao2);
		//glDrawArrays (GL_TRIANGLES, 0, numPoints2);
		// update other events like input handling 
		glfwPollEvents ();
		if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose (g_window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}