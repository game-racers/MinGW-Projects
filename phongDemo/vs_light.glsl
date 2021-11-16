#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
uniform mat4 projection_mat, view_mat, model_mat;
out vec3 position_eye, normal_eye;


void main () {
	position_eye = vertex_position;
	normal_eye =  vertex_normal;
	gl_Position = vec4(position_eye, 1.0);

}