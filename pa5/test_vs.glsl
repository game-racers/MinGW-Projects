/* shader to update a particle system based on a simple kinematics function */
#version 410 core

layout (location = 0) in vec3 v_i; // initial velocity
layout (location = 1) in float start_time;

uniform mat4 V, P;
uniform vec3 emitter_pos_wor; // emitter position in world coordinates
uniform float elapsed_system_time; // system time in seconds

// the fragment shader can use this for it's output colour's alpha component 
out float opacity;
out float x;
out float y;

void main() {
	// work out the elapsed time for _this particle_ after its start time
	float t = elapsed_system_time - start_time;
	// allow time to loop around so particle emitter keeps going
	t = mod (t, 7.0);
	opacity = 0.0;

	vec3 p = emitter_pos_wor;
	// gravity
	vec3 a = vec3 (0.0, -1.0, 0.0);
	// this is a standard kinematics equation of motion with velocity and
	// acceleration (gravity)
	//p += v_i * t + 0.5 * a * t * t;
	//p = vec3((t-3.5)/3.5, v_i[1]*sin(t), 0.0);
	p += vec3(v_i[0], sin(t), 0.0);
	x = p[0];
	y = p[1];
	// gradually make particle fade to invisible over 3 seconds
	//opacity = 1.0 - (t / 3.0);
	opacity = 0.5 - v_i[1]*sin(t);
	gl_Position = P * V * vec4 (p, 1.0);
	gl_PointSize = 15.0; // size in pixels
}
