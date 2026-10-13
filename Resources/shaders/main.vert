#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_InstanceOffset;
layout (location = 4) in vec3 a_Normal;
layout (location = 5) in vec4 a_Tangent;


uniform mat4 u_Model; 
uniform mat4 u_View;          
uniform mat4 u_Projection;
uniform mat3 u_NormalMatrix;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;

void main()
{
	vec4 modelPos = u_Model * vec4(a_Position, 1.0) + vec4(a_InstanceOffset, 0.0);
	vec4 viewPos = u_View * modelPos;

	// sent to fragment shader
	FragPos = vec3(viewPos);
	TexCoord = a_TexCoord;
	Normal = u_NormalMatrix * a_Normal;

	
	// Tangent
	mat3 meshToView = mat3(u_View * u_Model);
	vec3 T = meshToView * a_Tangent.xyz;
	
	vec3 N = normalize(Normal);
	T -= N * dot(T, N);
	T = normalize(T);


	// Bitangent
	float orientationSign = sign(determinant(meshToView)); // учитывает отражение при преобразовании объекта
	float signB = a_Tangent.w * orientationSign;
	vec3 B = cross(N, T) * signB;


	
	Tangent = T;
	Bitangent = B;


	gl_Position = u_Projection * viewPos;
}
