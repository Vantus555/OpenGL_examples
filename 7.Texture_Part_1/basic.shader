#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec2 TexCoord;

out vec4 m_color;
out vec2 v_TexCoord;

void main()
{
	gl_Position = position;
	v_TexCoord = TexCoord;
	m_color = Color;
};

#shader fragment
#version 330 core

out vec4 color;
in vec4 m_color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	vec4 Text = texture(u_Texture, v_TexCoord);
	color = Text;
};