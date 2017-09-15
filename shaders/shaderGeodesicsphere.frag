#version 330 core

uniform vec3 ucolor;

in vec3 colorCoord;

out vec4 color;

void main()
{
	//color = vec4(ucolor, 1.0f);
	color = vec4(colorCoord, 1.0f);
};