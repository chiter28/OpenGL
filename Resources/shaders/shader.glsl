#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 uTranslation;

void main()
{
	gl_Position = uTranslation * vec4(aPosition, 1.0);
	Color = aColor;
}



#type fragment
#version 450 core

in vec3 Color;
layout (location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(Color, 1.0);
}
