//g++ -o loadObjDemoCode loadObjDemoCode.cpp libglfw3dll.a libglew32.dll.a -I include -lglew32 -lglfw3 -lgdi32 -lopengl32

/******************************************************************************\
| OpenGL 2.1 Example Code.                                                     |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| Hello Triangle. Example back-ported to OpenGL 2.1 with GLSL 1.20             |
| If you're on Mac un-comment the version number code at the beginning. It     |
| will give you the latest, even if you say 3.2!                               |
| This uses the libraries GLEW and GLFW3 to start GL. Download and compile     |
| these first. Linking them might be a pain, but you'll need to master this.   |
\******************************************************************************/

#include <GL/glew.h>		/* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdlib.h> 

#define _USE_MATH_DEFINES

float tX = 0.0;

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E) { //can hold key down
		//cout << "Key pressed\n";
		tX += 0.1;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) { //only moves when key is initially hit
		//cout << "Key pressed\n";
		tX -= 0.1;
	}
}


int countLabel(string modelName, char label[]) {
	int numLab = 0;

	FILE *objFile;
	objFile = fopen(modelName.c_str(), "r");

	char buf[128];
	while (fscanf(objFile, "%s", &buf) != EOF) {
		if (strcmp(buf, label) == 0)
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

	//Fill-in: Init variables to keep track of min and max coordinates

	char buf[128];
	char label[] = "v";
	float a, b, c;
	float minX = 1000000000.0, minY = 1000000000.0, minZ = 1000000000.0, maxX = -1000000000.0, maxY = -1000000000.0, maxZ = -1000000000.0;
	while (fscanf(objFile, "%s", &buf) != EOF){ 
	    if (strcmp(buf,label) == 0){
		//cout << "vertex" << endl;
		fscanf(objFile, "%f %f %f\n", &a, &b, &c);
		//cout << "a,b,c: " << a << ", " << b << ", " << c << endl;
		verts[3*numVert + 0] = 1.0*a;
		verts[3*numVert + 1] = 1.0*b;
		verts[3*numVert + 2] = 1.0*c;
		if (a < minX)
			minX = a;
		if (a > maxX)
			maxX = a;
		if (b < minY)
			minY = b;
		if (b > maxY)
			maxY = b;
		if (c < minZ)
			minZ = c;
		if (c > minZ)
			maxZ = c;
		numVert++;
	    }
	}

	//Fill-in: Center object and scale to fit in window
	float scaleX = maxX - minX;
	float scaleY = maxY - minY;
	float scaleZ = maxZ - minZ;
	float transX = .5 * (maxX + minX);
	float transY = .5 * (maxY + minY);
	float transZ = .5 * (maxZ + minZ);

	float fullScale = max(scaleZ, max(scaleY, scaleX));

	for (int i = 0; i < numVert; i++) {
		verts[3 * i + 0] = (verts[3 * i + 0] - transX) / fullScale;
		verts[3 * i + 1] = (verts[3 * i + 1] - transY) / fullScale;
		verts[3 * i + 2] = (verts[3 * i + 2] - transZ) / fullScale;
	}
	fclose(objFile);
	cout << "Done loading vertices\n";
}




void loadFaces(string modelName, GLint faces[]){	
	cout << "Loading faces\n";
	int numFaces = 0;

	FILE *objFile;
	objFile = fopen(modelName.c_str(), "r");
	
	char buf[128];
	char label[] = "f";
	float a, b, c;
	while (fscanf(objFile, "%s", &buf) != EOF) {
		if (strcmp(buf, label) == 0) {
			//cout << "vertex" << endl;
			fscanf(objFile, "%f %f %f\n", &a, &b, &c);
			//cout << "a,b,c: " << a << ", " << b << ", " << c << endl;
			faces[3 * numFaces + 0] = a-1;
			faces[3 * numFaces + 1] = b-1;
			faces[3 * numFaces + 2] = c-1;
			numFaces++;
		}
	}
	fclose(objFile);
	cout << "Done loading faces\n";
}

void multMatrices(GLfloat m1[], GLfloat m2[], GLfloat result[]) {
	int row = 0;	// current row, honestly uneeded. 
	int col = 0;	// current col. Iterations of 4, resets to 0
	int index = 0;	// index of result matrix

	while (row < 4) {						// iterate through rows
		for (int i = 0; i < 4; i++) {			// iterate through columns
			result[index] = (m1[row] * m2[col] + m1[row + 4] * m2[col + 1] + m1[row + 8] * m2[col + 2] + m1[row + 12] * m2[col + 3]);
			col += 4;
			index += 4;
			if (index > 15)
				index = index - 15;
			if (col == 16)
				col = 0;
		}
		row++;
	}
}


int main() {
	//Read in obj
	string modelName = "objectFiles/dolphins.obj";

	int numVert = countLabel(modelName, "v");
	GLfloat* verts = new GLfloat[3*numVert];
	loadVertices(modelName, verts);

	int numFaces = countLabel(modelName, "f");
	GLint* faces = new GLint[3*numFaces];
	loadFaces(modelName, faces);

	//array for translation
	GLfloat trans[] = { 1.0, 0.0, 0.0, 0.0,  //first column
					   0.0, 1.0, 0.0, 0.0,  //second column
					   0.0, 0.0, 1.0, 0.0,  //third column
					   0.0, -0.5, 0.0, 1.0 }; //fourth column

	//array for rotation Z
	//float ang = 45.0*3.14159 / 180.0;
	GLfloat rot[] = { cos(angZ), sin(angZ), 0, 0, //first column
					-sin(angZ), cos(angZ), 0, 0, //second column
					0, 0, 1, 0, //third column
					0, 0, 0, 1 }; //fourth column


	GLfloat* points = new GLfloat[9*numFaces];
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
	}
	int numPoints = 3*numFaces;
	

	//Set-up window
	GLFWwindow *window = NULL;
	const GLubyte *renderer;
	const GLubyte *version;

	//Set-up VAO/VBO
	GLuint vao;
	GLuint vbo;
	
	const char *vertex_shader = "#version 410\n"
		"attribute vec3 vp;"
		"void main () {"
			"gl_Position =   vec4(vp, 1.0);"
		"}";
		

	const char *fragment_shader = "#version 410\n"
		"out vec4 frag_colour;"

		"void main () {"
		"	frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
		"}";
		
		
	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vs, fs;
	/* GL shader programme object [combined, to link] */
	GLuint shader_programme;

	
	/* start GL context and O/S window using the GLFW helper library */
	if ( !glfwInit() ) {
		fprintf( stderr, "ERROR: could not start GLFW3\n" );
		return 1;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );

	window = glfwCreateWindow( 640, 480, "Hello Triangle", NULL, NULL );
	glfwSetWindowPos(window, 500, 100);
	if ( !window ) {
		fprintf( stderr, "ERROR: could not open window with GLFW3\n" );
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent( window );
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	/* get version info */
	renderer = glGetString( GL_RENDERER ); /* get renderer string */
	version = glGetString( GL_VERSION );	 /* version as a string */
	printf( "Renderer: %s\n", renderer );
	printf( "OpenGL version supported %s\n", version );

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable( GL_DEPTH_TEST ); // enable depth-testing
														 // depth-testing interprets a smaller value as "closer"
	glDepthFunc( GL_LESS );

	/* a vertex buffer object (VBO) is created here. this stores an array of
		 data on the graphics adapter's memory. in our case - the vertex points */
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, 3*numPoints*sizeof(GLfloat)/*sizeof(points)/sizeof(*points) * sizeof( GLfloat )*/, points, GL_STATIC_DRAW );

	
	/* the vertex array object (VAO) is a little descriptor that defines which
	data from vertex buffer objects should be used as input variables to vertex
	shaders. in our case - use our only VBO, and say 'every three floats is a
	variable' */
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

	/* here we copy the shader strings into GL shaders, and compile them. we
		 then create an executable shader 'program' and attach both of the compiled
		 shaders. we link this, which matches the outputs of the vertex shader to
		 the inputs of the fragment shader, etc. and it is then ready to use */
	vs = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vs, 1, &vertex_shader, NULL );
	glCompileShader( vs );
	fs = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fs, 1, &fragment_shader, NULL );
	glCompileShader( fs );
	shader_programme = glCreateProgram();
	glAttachShader( shader_programme, fs );
	glAttachShader( shader_programme, vs );
	glLinkProgram( shader_programme );



	glPointSize(5.0);
	
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CW); // GL_CCW for counter clock-wise

	/* this loop clears the drawing surface, then draws the geometry described
		 by the VAO onto the drawing surface. we 'poll events' to see if the window
		 was closed, etc. finally, we 'swap the buffers' which displays our drawing
		 surface onto the view area. we use a double-buffering system which means
		 that we have a 'currently displayed' surface, and 'currently being drawn'
		 surface. hence the 'swap' idea. in a single-buffering system we would see
		 stuff being drawn
	one-after-the-other */
	while ( !glfwWindowShouldClose( window ) ) {
		// wipe the drawing surface clear
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		trans[12] = tX;

		// Identify callback functions
		glfwSetKeyCallback(window, key_callback); //for keyboard interaction
		//glfwSetMouseCallback(window, mouse_callback); //for mouse interaction

		GLfloat results[16];
		multMatrices(trans, rot, results);
		//GLfloat result2[16];
		//multMatrices(scale, result, result2);

		glUseProgram( shader_programme );
		glBindVertexArray( vao );
		// draw points 0-3 from the currently bound VAO with current shader
		glDrawArrays( GL_TRIANGLES, 0, numPoints); //(sizeof(points)/sizeof(*points))/3 );
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers( window );

	}


	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}

