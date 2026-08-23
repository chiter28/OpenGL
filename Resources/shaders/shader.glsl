#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uMVP;
out vec2 TexCoord;

void main()
{
	gl_Position = uMVP * vec4(aPosition, 1.0);
	TexCoord = aTexCoord;
}



#type fragment
#version 450 core

in vec2 TexCoord;
layout (location = 0) out vec4 outColor;

uniform sampler2D uSampler;
 
void main()
{
	vec4 tex = texture(uSampler, TexCoord);
	outColor = tex;	
}
