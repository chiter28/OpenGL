#type vertex
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_InstanceOffset;
layout (location = 4) in vec3 a_Normal;


uniform mat4 u_Model; 
uniform mat4 u_View;          
uniform mat4 u_Projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	vec4 modelPos = u_Model * vec4(a_Position, 1.0) + vec4(a_InstanceOffset, 0.0);

	vec4 viewPos = u_View * modelPos;


	FragPos = vec3(viewPos);
	
	mat3 normalMatrix = transpose(inverse(mat3(u_View * u_Model)));
	Normal = normalMatrix * a_Normal;
	
	TexCoord = a_TexCoord;
		
	gl_Position = u_Projection * viewPos;
}





#type fragment
#version 450 core

layout (location = 0) out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Light
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	float SpecularIntensity;
	vec3 Direction;
};

struct Material
{
	vec4 BaseColorFactor;
	vec3 SpecularColor;
	float Shininess;
};

uniform sampler2D u_AlbedoMap;

uniform Light u_Light;
uniform Material u_Material;
 
void main()
{

	vec3 ambientLight = u_Light.Color * u_Light.AmbientIntensity;
	vec3 diffuseLight =  vec3(0.0, 0.0, 0.0);
	vec3 specularLight = vec3(0.0, 0.0, 0.0);

	float diffuseFactor = max(dot(normalize(Normal), u_Light.Direction), 0.0);

	if (diffuseFactor > 0)
	{
		diffuseLight = u_Light.Color * diffuseFactor * u_Light.DiffuseIntensity;

		vec3 viewDir = normalize(-FragPos);
		vec3 halfwayDir = normalize(u_Light.Direction + viewDir);
		float specularFactor = pow(max(dot(normalize(Normal), halfwayDir), 0.0), u_Material.Shininess);
		if (specularFactor > 0.0)
		{
			specularLight = u_Light.Color * u_Light.SpecularIntensity * specularFactor * u_Material.SpecularColor;
		}
	}



	vec4 baseColor = texture(u_AlbedoMap, TexCoord) * u_Material.BaseColorFactor;
	vec3 color = baseColor.rgb * (ambientLight + diffuseLight) + specularLight; 

	FragColor = vec4(color, baseColor.a);
}
