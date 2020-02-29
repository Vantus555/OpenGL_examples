#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
out vec4 Color;

void main()
{
	Color = color;
	gl_Position = position;
};

#shader fragment
#version 330 core

in vec4 Color;
out vec4 FragColor;

void main()
{
	FragColor = Color;
};