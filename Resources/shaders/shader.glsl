#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 a_InstanceOffset;

uniform mat4 uMVP;
out vec2 TexCoord;

void main()
{
	vec3 finalPosition = aPosition + a_InstanceOffset;

	gl_Position = uMVP * vec4(finalPosition, 1.0);
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
