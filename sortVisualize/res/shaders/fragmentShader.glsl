#version 330 core

in vec4 vg_fcolor;
out vec4 FragColor;
uniform float u_Time

void main()
{
	FragColor = vg_fcolor * u_Time;
}
