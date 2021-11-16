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
//g++ -o textMap gl_utils.cpp math_funcs.cpp stb_image.c textMap.cpp libglfw3dll.a libglew32.dll.a -I include -lglew32 -lglfw3 -lgdi32 -lopengl32

#include "math_funcs.h"
#include "gl_utils.h"
#include "stb_image.h" // Sean Barrett's image loader - http://nothings.org/
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


bool load_texture (const char* file_name, GLuint* tex) {
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load (file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf (stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf (
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	glGenTextures (1, tex);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, *tex);
	glTexImage2D (
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap (GL_TEXTURE_2D);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
	return true;
}



int main () {
	
	
	
	
	restart_gl_log ();
	// start GL context and O/S window using the GLFW helper library
	start_gl ();
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

	/* OTHER STUFF GOES HERE NEXT */
	GLfloat points[] = {
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
	};
	
	GLfloat normals[] = {
		1.0f, -1.0f,  -1.0f,
		1.0f, 0.0f,  -1.0f,
		1.0f, 1.0f,  -1.0f,
		1.0f, 1.0f,  -1.0f,
		1.0f, 0.0f,  -1.0f,
		1.0f, -1.0f,  -1.0f
	};
	
	GLfloat texcoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};
	int numPoints = 6;
	
	
	GLuint points_vbo;
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * numPoints * sizeof (GLfloat), points, GL_STATIC_DRAW);
	
	GLuint normals_vbo;
	glGenBuffers (1, &normals_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * numPoints * sizeof (GLfloat), normals, GL_STATIC_DRAW);
	
	GLuint texcoords_vbo;
	glGenBuffers (1, &texcoords_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, texcoords_vbo);
	glBufferData (GL_ARRAY_BUFFER, 2 * numPoints * sizeof (GLfloat), texcoords, GL_STATIC_DRAW);
	
	
	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, texcoords_vbo);
	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL); 
	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	glEnableVertexAttribArray (2);
	
	
	GLuint shader_programme = create_programme_from_files (
		"test_vs_textMap.glsl", "test_fs_textMap.glsl");
	
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

	
	/* create VIEW MATRIX */
	float cam_pos[] = {0.0f, 0.0f, 2.0f}; // don't start at zero, or we will be too close
	float cam_yaw = 0.0f; // y-rotation in degrees
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;
	
	/* matrix for moving the triangle */
	mat4 model_mat = identity_mat4 ();
	
	
	// load texture
	GLuint tex;
	assert (load_texture ("mand.png", &tex));
	
	
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