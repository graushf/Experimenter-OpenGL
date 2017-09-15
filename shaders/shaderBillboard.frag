#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D textureBillboard;

void main()
{
	//color = vec4(1.0, 0.0, 0.0, 1.0);
	//
	color = texture(textureBillboard, vec2(TexCoord.x, 1 - TexCoord.y));
	//color = vec4(TexCoord, 1.0, 1.0);
}