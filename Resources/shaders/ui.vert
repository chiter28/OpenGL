#version 450 core

layout (location = 0) in vec3 a_Position;

uniform vec4 u_Rect;
uniform mat4 u_Projection;

void main()
{
	vec2 screenPosition = u_Rect.xy + a_Position.xy * u_Rect.zw;
	gl_Position = u_Projection * vec4(screenPosition, 0.0, 1.0);
}
