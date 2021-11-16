//g++ -o helloTriangle helloTriangle_TR-1.cpp libglfw3dll.a libglew32.dll.a -I include -lglew32 -lglfw3 -lgdi32 -lopengl32

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <math.h> //math library for sin's and cos's
#include <iostream>


using namespace std;

// Global variables for transformations
float tX = 0.0;
float tY = 0.0;
float tZ = 0.0;

int triangleCount = 10;

float angX = 0.0*3.14159 / 180.0;
float angY = 0.0*3.14159 / 180.0;
float angZ = 0.0*3.14159 / 180.0;

float sX = 1;
float sY = 1;
float sZ = 1;

float skew = 90.0*3.14159 / 180.0;

bool orthoBool = true;

//Write callback functions at beginning of program
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_D && action == GLFW_PRESS){ //can hold key down
		//cout << "Key pressed\n";
		tX += 0.1;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		tX -= 0.1;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS){ //can hold key down
		//cout << "Key pressed\n";
		tY += 0.1;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS){ //only moves when key is initially hit
		//cout << "Key pressed\n";
		tY -= 0.1;
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) { //can hold key down
		//cout << "Key pressed\n";
		tZ += 0.1;
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		tZ -= 0.1;
	}
	// Angle changes
	if (key == GLFW_KEY_R && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		angX += 15.0*3.14159 / 180.0;
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		angX -= 15.0*3.14159 / 180.0;
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		angZ += 15.0*3.14159 / 180.0;
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		angZ -= 15.0*3.14159 / 180.0;
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		angY += 15.0*3.14159 / 180.0;
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		angY = 15.0*3.14159 / 180.0;
	}
	// Scale changes
	if (key == GLFW_KEY_U && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		sX += .1;
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		sX -= .1;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		sY += .1;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		sY -= .1;
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		sZ += .1;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		sZ -= .1;
	}
	// Skew
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		skew += 15.0*3.14159 / 180.0;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		skew -= 15.0*3.14159 / 180.0;
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		if (orthoBool)
			orthoBool = false;
		else
			orthoBool = true;
	}
}

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
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
  } 

  // uncomment these lines if on Apple OS X
  /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
  if (!window) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  
  float a = 5.0;
  cout << "this is a test print statment\n";
  cout << "this is a test print statment" << endl;
  cout << "val a: " << a << endl;
                                  
  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"


  glPointSize(5.0f);
  glClearColor(0.0f,0.0f,0.0f,1.0f);

  /* OTHER STUFF GOES HERE NEXT */
	float points[] = {
	    0.0f,  0.6f,  0.0f,
	    0.1f, 0.0f,  0.0f,
	    0.0f, 0.0f,  0.1f,	// first
	    0.0f,  0.6f,  0.0f,
	    -0.1f, 0.0f,  0.0f,
	    0.0f, 0.0f,  0.1f,	// second
	    0.0f,  0.6f,  0.0f,
	    0.1f, 0.0f,  0.0f,
	    0.0f, 0.0f,  -0.1f,	// third
	    0.0f,  0.6f,  0.0f,
	    -0.1f, 0.0f,  0.0f,
	    0.0f, 0.0f,  -0.1f,	// fourth		End of the blade
	    0.0f, 0.0f, 0.2f, 
	    0.0f, 0.0f, -0.2f,
	    -0.3f, 0.0f, 0.0f,	// fifth		start of crossGuard, top part and not visible in normal view
	    0.0f, 0.0f, 0.2f,
	    0.3f, 0.0f, 0.0f,
	    0.0f, -0.1f, 0.0f,	// sixth		right front
	    0.0f, 0.0f, -0.2f,
	    0.3f, 0.0f, 0.0f,
	    0.0f, -0.1f, -0.0f,	// seventh		right back
		0.0f, 0.0f, 0.2f,
	    0.0f, 0.0f, -0.2f,
	    -0.3f, 0.0f, 0.0f,	// eighth		top left, not visible
		0.0f, 0.0f, 0.2f,
		-0.3f, 0.0f, 0.0f,
		0.0f, -0.1f, 0.0f,	// ninth		right front
		0.0f, 0.0f, -0.2f,
		-0.3f, 0.0f, 0.0f,
		0.0f, -0.1f, -0.0f,	// tenth		right back
	};

	float points2[] = {
	   0.0f,  0.6f,  0.0f,
	   -0.1f, 0.0f,  0.0f,
	   0.0f, .4f,  0.1f,
	   0.1f,  0.6f,  0.0f
	};
	
	//array called trans
	GLfloat trans[] = {1.0, 0.0, 0.0, 0.0,  //first column
					   0.0, 1.0, 0.0, 0.0,  //second column
					   0.0, 0.0, 1.0, 0.0,  //third column
					   tX, tY, tZ, 1.0}; //fourth column
					   
	//array for rotation
	//float ang = 0.0*3.14159/180.0;
	GLfloat rotX[] = {1, 0, 0, 0, //first column
					0, cos(angX), sin(angX), 0, //second column
					0, -sin(angX), cos(angX), 0, //third column
					0, 0, 0, 1}; //fourth column

	GLfloat rotY[] = { cos(angY), 0, -sin(angY), 0, //first column
					0, 1, 0, 0, //second column
					sin(angY), 0, cos(angY), 0, //third column
					0, 0, 0, 1 }; //fourth column

	GLfloat rotZ[] = { cos(angZ), sin(angZ), 0, 0, //first column
					-sin(angZ), cos(angZ), 0, 0, //second column
					0, 0, 1, 0, //third column
					0, 0, 0, 1 }; //fourth column

	GLfloat scale[] = { sX, 0, 0, 0,
						0, sY, 0, 0,
						0, 0, sZ, 0,
						0, 0, 0, 1 };

	GLfloat skewMat[] = { 1, 0, 0, 0,
						1 / tan(skew), 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1 };

	GLfloat orthoMat[] = {  1,0,0,0,
							0,1,0,0,
							0,0,0,0,
							0,0,0,1 };

	float range = tan(67.0 * 3.14159 / 180.0 * 0.5) * 0.01;
	float aspect = 1.0f;
	float far = 100.0f;
	float near = 0.01f;
	float Sx = ((2 * near) / (range * aspect + range * aspect));
	float Sy = (near/ range);
	float Sz = -((far + near) / (far - near));
	float Pz = -((2 * far * near) / (far - near));

	GLfloat persMat[] = { Sx, 0.0, 0.0, 0.0,
						0.0, Sy, 0.0, 0.0,
						0.0, 0.0, Sz, -1.0,
						0.0, 0.0, Pz, 0.0 };

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * triangleCount * sizeof(float), points, GL_STATIC_DRAW);

	

	GLuint vbo1 = 1;
	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), points2, GL_STATIC_DRAW);



	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);



	GLuint vao1 = 0;
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	
	
	const char* vertex_shader =
	"#version 400\n"
	"in vec3 vp;"
	"uniform mat4 t1;"
	"uniform mat4 r1;"
	"uniform mat4 p;"
	"void main() {"
	"  gl_Position = t1*r1*p*vec4(vp.x, vp.y, vp.z, 1.0);"
	"}";

	const char* fragment_shader =
	"#version 400\n"
	"out vec4 frag_colour;"
	"void main() {"
	"  frag_colour = vec4(0.7, 1.0, 0.4, 1.0);"
	"}";	


	const char* fragment_shader1 =
	"#version 400\n"
	"out vec4 frag_colour;"
	"void main() {"
	"  frag_colour = vec4(1.0, 0.0, 0.0, 1.0);"
	"}";	


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);


	GLuint fs1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs1, 1, &fragment_shader1, NULL);
	glCompileShader(fs1);

	GLuint shader_programme1 = glCreateProgram();
	glAttachShader(shader_programme1, fs1);
	glAttachShader(shader_programme1, vs);
	glLinkProgram(shader_programme1);
	
	
			  
	int logLen;
	GLchar* log;
	glGetProgramiv(shader_programme1, GL_INFO_LOG_LENGTH, &logLen);

	
	
	/*if(logLen > 0) {

		// Show any errors as appropriate, uncomment if brave...

		glGetProgramInfoLog(shader_programme1, logLen, &logLen, log);

		fprintf(stderr, "Prog Info Log: %s\n", log);

	}*/
			
	cout << "\nW for translate +Y / S for translate -Y\n";
	cout << "D for translate +Y / A for translate -X\n";
	cout << "Q for translate +Z / E for translate -Z\n";
	cout << "R for rotate 15* about the X-axis / F for -15*\n";
	cout << "T for rotate 15* about the Z-axis / G for -15*\n";
	cout << "Y for rotate 15* about the Y-axis / H for -15*\n";
	cout << "U to scale up the X - axis / J to scale down\n";
	cout << "I to scale up the Y - axis / K to scale down\n";
	cout << "O to scale up the Z - axis / L to scale down\n";
	cout << "Z to skew 15* about the X - axis / X for -15*\n";
	cout << "P to change the perspective between Perspective view and Orthogonal\n";

	while(!glfwWindowShouldClose(window)) {
		// update transformation matrices
		trans[12] = tX;
		trans[13] = tY;
		trans[14] = tZ;

		rotX[5] = cos(angX);
		rotX[6] = sin(angX);
		rotX[9] = -sin(angX);
		rotX[10] = cos(angX);
		
		rotY[0] = cos(angY);
		rotY[2] = -sin(angY);
		rotY[8] = sin(angY);
		rotY[10] = cos(angY);

		rotZ[0] = cos(angZ);
		rotZ[1] = sin(angZ);
		rotZ[4] = -sin(angZ);
		rotZ[5] = cos(angZ);

		scale[0] = sX;
		scale[5] = sY;
		scale[10] = sZ;

		skewMat[1] = 1.0/tan(skew);

		GLfloat results0[16];
		multMatrices(scale, skewMat, results0);

		GLfloat results1[16];
		multMatrices(rotX, results0, results1);

		GLfloat results2[16];
		multMatrices(rotY, results1, results2);

		GLfloat results3[16];
		multMatrices(rotZ, results2, results3);


	  	int t1_mat_location = glGetUniformLocation (shader_programme, "t1");
		glUseProgram( shader_programme );
		glUniformMatrix4fv (t1_mat_location, 1, GL_FALSE, trans);
		
		int r1_mat_location = glGetUniformLocation (shader_programme, "r1");
		glUseProgram( shader_programme );
		glUniformMatrix4fv (r1_mat_location, 1, GL_FALSE, results3);

		if (orthoBool) {
			int p_mat_location = glGetUniformLocation(shader_programme, "p");
			glUseProgram(shader_programme);
			glUniformMatrix4fv(p_mat_location, 1, GL_FALSE, orthoMat);
		}
		else if (!orthoBool) {
			int p_mat_location = glGetUniformLocation(shader_programme, "p");
			glUseProgram(shader_programme);
			glUniformMatrix4fv(p_mat_location, 1, GL_FALSE, persMat);
		}
		
		
		// Identify callback functions
		glfwSetKeyCallback(window, key_callback); //for keyboard interaction
		//glfwSetMouseCallback(window, mouse_callback); //for mouse interaction
		
		
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		glPointSize(15.0);
		// draw points 0-number from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0,  3 * triangleCount);

		glUseProgram(shader_programme1);
		glBindVertexArray(vao1);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 4);
		// update other events like input handling 
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}

