#version 410

in vec3 position_eye, normal_eye, pt_pos;

uniform mat4 view_mat;

// fixed point light properties
vec3 light_position_world  = vec3 (0.0, 1.0, 1.0);
vec3 light_direction = vec3 (0.0, -1.0, 0.0);
vec3 warmColour = vec3 (0.0, 0.12549, 0.76078); // cobalt blue
vec3 coolColour = vec3 (0.96471, 0.13333, 0.0902); // ruby red
float angle = 45.0; //
float alpha, cosAlpha, wk, r, g, b;
vec3 v1;




out vec4 fragment_colour; // final colour of surface

void main () {
	//distance between light and point
	float dist = sqrt( (pt_pos[0]-light_position_world[0])*(pt_pos[0]-light_position_world[0]) + 
					   (pt_pos[1]-light_position_world[1])*(pt_pos[1]-light_position_world[1]) + 
					   (pt_pos[2]-light_position_world[2])*(pt_pos[2]-light_position_world[2]) );
	
	// alpha computation along with translation to degrees.
	//v1 = vec3 (pt_pos[0] - light_position_world[0], pt_pos[1] - light_position_world[1], pt_pos[2] - light_position_world[2]);
	
	float vecAng = ((normal_eye[0] * light_direction[0]) + (normal_eye[1] * light_direction[1]) + 
					(normal_eye[2] * light_direction[2])) / sqrt(light_direction[0] * 
					light_direction[0] + light_direction[1] * light_direction[1] + 
					light_direction[2] * light_direction[2]);
	
	float alpha = acos(vecAng) * (180.0 / 3.14159);
	wk = 0.5 * (1 + vecAng);
	
	//mix warm and cool colours
	r = wk * (warmColour[0]) + (1 - wk) * coolColour[0];
	g = wk * (warmColour[1]) + (1 - wk) * coolColour[1];
	b = wk * (warmColour[2]) + (1 - wk) * coolColour[2];
	
	// final colour
	fragment_colour = vec4 (r, g, b, 1.0);
}