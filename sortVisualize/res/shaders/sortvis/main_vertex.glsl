#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 color; // the position variable has attribute position 0

out vec3 v_Color;

uniform mat4 u_MVP;

void main()
{
    vec4 newPos = u_MVP * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
	gl_Position = vec4(newPos.xy, 1.0, 1.0);
	v_Color = color;
}