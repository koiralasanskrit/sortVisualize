#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 color;

out vec3 v_Color;
out vec4 v_Pos;

uniform mat4 u_MVP;

void main()
{
    vec4 newPos = u_MVP * vec4(aPos, 1.0);
	gl_Position = vec4(newPos.xy, 1.0, 1.0);
	v_Color = color;
	v_Pos = gl_Position;
}