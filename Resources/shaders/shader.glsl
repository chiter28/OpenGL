#type vertex
#version 450 core

uniform mat4 uTranslation;

layout (location = 0) in vec3 Position;

void main()
{
	gl_Position = uTranslation * vec4(Position, 1.0);
}



#type fragment
#version 450 core

layout (location = 0) out vec4 o_Color;

void main()
{
	o_Color = vec4(1.0, 0.3, 0.7, 1.0);
}
