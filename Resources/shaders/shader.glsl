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

// Output variables
layout (location = 0) out vec4 FragColor;


// Input variables
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;


// Structs
struct Light
{
	vec3 Color;
	float AmbientIntensity;
	float Intensity;
	vec3 Direction;
};

// UBO
layout(std140) uniform MaterialBlock
{
	vec4 BaseColor;
	float Metallic;
	float Roughness;
} u_Material;

// Uniforms
uniform Light u_Light;


// Debug
uniform int u_DebugView;

const int DEBUG_BASE_COLOR = 1;
const int DEBUG_NORMALS = 2;
const int DEBUG_METALLIC = 3;
const int DEBUG_ROUGHNESS = 4;




uniform sampler2D u_AlbedoMap;
uniform sampler2D u_MetallicRoughnessMap;


// Constants
const float PI = 3.14159265359;



// Functions
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	float t = 1.0 - clamp(cosTheta, 0.0, 1.0);
	float weight = pow(t, 5.0);
	
	return F0 + (vec3(1.0) - F0) * weight;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	// Roughness
	float r = clamp(roughness, 0.05, 1.0);
	float alpha = r * r;
	float alphaSquare = alpha * alpha;

	// Angle between N ^ H
	float NdotH = clamp(dot(N, H), 0.0, 1.0);
	float NdotHSquare = NdotH * NdotH;

	float denominator = mix(1.0, alphaSquare, NdotHSquare);
	return alphaSquare / (PI * pow(denominator, 2.0));
	
	// mix(a, b, t) = a * (1.0 - t) + b * t;
	// cos²(90)(|_) = 0			-> 1
	// cos²(75)(|/) = 0.067 	-> 1 * 0.93 + alphaSquare * 0.067  
	// cos²(60)(|/) = 0.25      -> 1 * 0.75 + alphaSquare * 0.25
	// cos²(30)(|/) = 0.75		-> 1 * 0.25 + alphaSquare * 0.75
	// cos²(0)(||) = 1			-> alphaSquare
}


float GeometrySmithG1GGX(float NdotV, float roughness)
{
	// Roughness
	float r = clamp(roughness, 0.05, 1.0);
	float alpha = r * r;
	float alphaSquare = alpha * alpha;

	float c = clamp(NdotV, 0.0, 1.0);
	float cSquare = c * c;

	float rootArg = mix(alphaSquare, 1.0, cSquare); 
	return (2.0 * c) / (c + sqrt(rootArg)); // от 0 до 1
		
	// mix(a, b, t) = a * (1.0 - t) + b * t;
	// cos²(90)(|_) = 0			-> alphaSquare
	// cos²(75)(|/) = 0.067		-> alphaSquare * 0.93 + 0.067  
	// cos²(60)(|/) = 0.25      -> alphaSquare * 0.75 + 0.25
	// cos²(30)(|/) = 0.75		-> alphaSquare * 0.25 + 0.75
	// cos²(0)(||) = 1			-> 1
}







void main()
{

	vec4 baseColor = texture(u_AlbedoMap, TexCoord) * u_Material.BaseColor; 
	vec4 metallic_roughness = texture(u_MetallicRoughnessMap, TexCoord);
		
	float metallic = u_Material.Metallic * metallic_roughness.b;
	float roughness = u_Material.Roughness * metallic_roughness.g;
	
	vec3 N = normalize(Normal);
	

	// Debug
	if (u_DebugView == DEBUG_BASE_COLOR)
	{
		FragColor = vec4(baseColor.rgb, 1.0);
		return;
	}
	if (u_DebugView == DEBUG_NORMALS)
	{
		FragColor = vec4(N * 0.5 + 0.5, 1.0);
		return;
	}
	if (u_DebugView == DEBUG_METALLIC)
	{
		FragColor = vec4(vec3(metallic), 1.0);
		return;
	}
	if (u_DebugView == DEBUG_ROUGHNESS)
	{
		FragColor = vec4(vec3(roughness), 1.0);
		return;
	}



	vec3 F0 = mix(vec3(0.04), baseColor.rgb, metallic);

	vec3 L = normalize(u_Light.Direction);
	vec3 V = normalize(-FragPos);

	vec3 ambientLight = u_Light.Color * u_Light.AmbientIntensity;
	vec3 diffuseLight =  vec3(0.0, 0.0, 0.0);
	vec3 specularLight = vec3(0.0, 0.0, 0.0);

	float NdotL = max(dot(N, L), 0.0); // cos(l)
	float NdotV = max(dot(N, V), 0.0); // cos(l)

	if (NdotL > 0.00001 && NdotV > 0.00001)
	{

		 // Такую нормаль должна иметь микрогрань, чтобы отразить свет от источника к камере
		vec3 H = normalize(L + V);

		// Доля энергии света, падающего из направления L, которую микрогрань с нормалью - H отразит зеркально в сторону V
		vec3 F = FresnelSchlick(dot(H, V), F0);

		// Плотность распределения микрограней имеющих нормали H
		float D = DistributionGGX(N, H, roughness);

		// Коэффициент перекрития луча от поверхности к камере из-за перекрития выпуклостями микрограней
		// G1 = 1 - не перекривает // G1 = 0 - полностью перекривает
		float G1V = GeometrySmithG1GGX(NdotV, roughness);
		// Коэффициент перекрития луча от источника к поверхности из-за перекрития выпуклостями микрограней
		float G1L = GeometrySmithG1GGX(NdotL, roughness);
		// Разделимая модель Смита: учитываем перекрытие в обоих направлениях.
		float G = G1V * G1L;

		vec3 lightIrradiance = u_Light.Color * u_Light.Intensity;
		vec3 specularBRDF = (D * F * G) / (4 * NdotL * NdotV);

		// Оставшаяся часть енергии света которая не отразилася от микрограни
		vec3 diffuseWeight = mix((vec3(1.0) - F), vec3(0.0), metallic); 
		// mix(a, b, t) = a * (1.0 - t) + b * t;
		vec3 diffuseBRDF = diffuseWeight * baseColor.rgb / PI;

		diffuseLight = diffuseBRDF * lightIrradiance * NdotL;
		specularLight = specularBRDF * lightIrradiance * NdotL;

	}


	

	vec3 color = baseColor.rgb * ambientLight + diffuseLight + specularLight; 

	FragColor = vec4(color, baseColor.a);
}



	
