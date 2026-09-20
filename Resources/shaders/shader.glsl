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
uniform mat3 u_NormalMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	vec4 modelPos = u_Model * vec4(a_Position, 1.0) + vec4(a_InstanceOffset, 0.0);
	vec4 viewPos = u_View * modelPos;

	// sent to fragment shader
	FragPos = vec3(viewPos);
	Normal = u_NormalMatrix * a_Normal;
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
	float Intensity;
	vec3 Direction;
};

struct Material
{
	vec4 BaseColorFactor;
	vec3 SpecularColor;
	float Shininess;

	float Metallic;
	float Roughness;
};

uniform sampler2D u_AlbedoMap;

uniform Light u_Light;
uniform Material u_Material;
uniform bool u_Has_AlbedoMap;
 
void main()
{

	vec3 ambientLight = u_Light.Color * u_Light.AmbientIntensity;
	vec3 diffuseLight =  vec3(0.0, 0.0, 0.0);
	vec3 specularLight = vec3(0.0, 0.0, 0.0);

	float diffuseFactor = max(dot(normalize(Normal), u_Light.Direction), 0.0); // cos(l)

	if (diffuseFactor > 0.00001)
	{
		diffuseLight = u_Light.Color * diffuseFactor * u_Light.Intensity;

		vec3 viewDir = normalize(-FragPos);
		vec3 halfwayDir = normalize(u_Light.Direction + viewDir);
		float specularFactor = pow(max(dot(normalize(Normal), halfwayDir), 0.0), u_Material.Shininess);
		if (specularFactor > 0.0)
		{
			specularLight = u_Light.Color * specularFactor * u_Light.Intensity * u_Material.SpecularColor;
		}
	}


	vec4 baseColor = u_Material.BaseColorFactor;

	if (u_Has_AlbedoMap)
	{
		 baseColor *= texture(u_AlbedoMap, TexCoord); 
	}

	vec3 color = baseColor.rgb * (ambientLight + diffuseLight) + specularLight; 

	FragColor = vec4(color, baseColor.a);
}
