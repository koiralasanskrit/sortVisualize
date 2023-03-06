#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in float thikness;

out vec4 v_fcolor;
out float v_thickness;

uniform mat4 u_MVP;

void main() {
	gl_Position = vec4(aPos, 1.0);
	v_fcolor = vec4(color, 1.0);
	v_thickness = thikness;
}
