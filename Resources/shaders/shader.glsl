#type vertex
#version 450 core

uniform float u_Scale;

layout (location = 0) in vec3 Position;

void main()
{
	gl_Position = vec4(u_Scale * Position.x, u_Scale * Position.y, u_Scale * Position.z, 1.0);
}



#type fragment
#version 450 core

layout (location = 0) out vec4 o_Color;

void main()
{
	o_Color = vec4(1.0, 0.3, 0.7, 1.0);
}
