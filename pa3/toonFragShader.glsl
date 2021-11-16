#version 410

in vec3 position_eye, normal_eye, pt_pos;

uniform mat4 view_mat;

// fixed point light properties
vec3 light_position_world  = vec3 (0.0, 10.0, 10.0);
vec3 light_direction = vec3 (0.0, -1.0, 0.0);
vec3 cameraDir = vec3 (0.0, 0.0, -1.0);
float theta, cosTheta, cosShade, shade;
vec3 v1, c1;

out vec4 fragment_colour; // final colour of surface

void main () {
	// theta computation along with translation to degrees.
	v1 = vec3 (light_position_world[0] - pt_pos[0], light_position_world[1] - pt_pos[1], light_position_world[2] - pt_pos[2]);
	
	cosTheta = ((normal_eye[0] * v1[0]) + (normal_eye[1] * v1[1]) + (normal_eye[2] * v1[2])) / 
				sqrt(v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2]); 
	
	theta = acos(cosTheta) * (180.0 / 3.14159);
	
	// toon shading
	//c1 = vec3 (cameraDir[0] - pt_pos[0], cameraDir[1] - pt_pos[1], cameraDir[2] - pt_pos[2]);
	//cosShade = (c1[0] * normal_eye[0] + c1[1] * normal_eye[1] + c1[2] * normal_eye[2]) / 
	//			sqrt(c1[0] * c1[0] + c1[1] * c1[1] + c1[2] * c1[2]); 
	cosShade = (cameraDir[0] * normal_eye[0] + cameraDir[1] * normal_eye[1] + cameraDir[2] * normal_eye[2]) / 
				sqrt(cameraDir[0] * cameraDir[0] + cameraDir[1] * cameraDir[1] + cameraDir[2] * cameraDir[2]);
	
	shade = acos(cosShade) * (180.0 / 3.14159);
	
	// final colour
	if (theta < 10.0) {
		fragment_colour = vec4(0.76078, 0.87451, 1.0, 1.0); 
	}else if (theta < 35.0) {
		fragment_colour = vec4(0.4, 0.59608, 1.0, 1.0); 
	}else if (theta < 50.0) {
		fragment_colour = vec4(0.16863, 0.39608, 0.92549, 1.0); 
	}else {
		fragment_colour = vec4(0.08235, 0.19216, 0.49412, 1.0); 
	}
	if (shade < 90.0 && shade > 70.0) {
		fragment_colour = vec4(0.3, 0.3, 0.3, 1.0);
	} 
	if (shade < 20.0 && shade > 0.0) {
		fragment_colour = vec4(0.3, 0.3, 0.3, 1.0);
	}
}