//g++ -o helloTriangle helloTriangle_TR.cpp libglfw3dll.a libglew32.dll.a -I include -lglew32 -lglfw3 -lgdi32 -lopengl32

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <math.h> //math library for sin's and cos's
#include <iostream>


using namespace std;

int main() {
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
  } 


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
	   -0.9f,  0.9,  0.0f,
	   0.5f, 0.0f,  0.0f,
	   -0.333f, -0.88f,  0.0f
	};

	float points2[] = {
	   0.1f,  0.6f,  0.0f,
	   0.6f, -0.4f,  0.0f,
	  -0.4f, -0.4f,  0.0f,
	  0.1f,  0.6f,  0.0f
	};
	
	float colours[] = {
		73/255.0, 56/255.0, 41/255.0,
		102/255.0, 141/255.0, 60/255.0,
		131/255.0, 146/255.0, 159/255.0
	}; 

	//array called trans
	GLfloat trans[] = {1.0, 0.0, 0.0, 0.0,  //first column
					   0.0, 1.0, 0.0, 0.0,  //second column
					   0.0, 0.0, 1.0, 0.0,  //third column
					   0.0, -0.5, 0.0, 1.0}; //fourth column
					   
	//array for rotation
	float ang = 45.0*3.14159/180.0;
	GLfloat rot[] = {cos(ang), sin(ang), 0, 0, //first column
					-sin(ang), cos(ang), 0, 0, //second column
					0, 0, 1, 0, //third column
					0, 0, 0, 1}; //fourth column
	

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);

	

	GLuint vbo1 = 1;
	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), points2, GL_STATIC_DRAW);



	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);



	GLuint vao1 = 0;
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	const char* vertex_shader =
	"#version 400\n"
	"layout(location = 0) in vec3 vertex_position;"
	"layout(location = 1) in vec3 vertex_colour;"
	"out vec3 colour;"
	//"in vec3 vp;"
	//"uniform mat4 t1;"
	//"uniform mat4 r1;"
	"void main() {"
	"  colour = vertex_colour;"
	"  gl_Position = vec4(vertex_position, 1.0);"
	"}";

	const char* fragment_shader =
	"#version 400\n"
	"in vec3 colour;"
	"out vec4 frag_colour;"
	"void main() {"
	"  frag_colour = vec4(colour, 1.0);"
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
	// insert location binding code here
	glBindAttribLocation(shader_programme, 0, "vertex_position");
	glBindAttribLocation(shader_programme, 1, "vertex_colour");
	
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

		// Show any errors as appropriate

		glGetProgramInfoLog(shader_programme1, logLen, &logLen, log);

		fprintf(stderr, "Prog Info Log: %s\n", log);

	}*/
			

			  
	while (!glfwWindowShouldClose(window)) {
		int t1_mat_location = glGetUniformLocation(shader_programme, "t1");
		glUseProgram(shader_programme);
		glUniformMatrix4fv(t1_mat_location, 1, GL_FALSE, trans);

		int r1_mat_location = glGetUniformLocation(shader_programme, "r1");
		glUseProgram(shader_programme);
		glUniformMatrix4fv(r1_mat_location, 1, GL_FALSE, rot);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/*glUseProgram(shader_programme1);
		glBindVertexArray(vao1);
		glDrawArrays(GL_TRIANGLES, 0, 4);
		*/
		glfwPollEvents();
		glfwSwapBuffers(window);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}
	}

    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}