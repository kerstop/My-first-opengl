#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec3 objSpacePos;

void main()
{
	FragColor = vec4((objSpacePos + 1.0) / 2.0, 1.0);
}