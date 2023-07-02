#version 330 core

#define MAX_POINT_LIGHTS 4

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aBiTangent;

uniform vec3 viewPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform int texture_normal_set;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

//out vec3 adjustedFragPosLightSpace;

out vec3 ViewPos;
out mat3 TBN;
out DirectionalLightVSOut 
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} directionalLightVSOut;

out PointLightVSOut 
{
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
} pointLightsVSOut[MAX_POINT_LIGHTS];

void main()
{
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	// TODO check out why is this like this
	Normal = mat3(transpose(inverse(model))) * aNormal;
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	//T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(vec3(model * vec4(aBiTangent, 0.0)));//cross(N, T);
	
	TBN = transpose(mat3(T, B, N));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	directionalLightVSOut.direction = directionalLight.direction;
	directionalLightVSOut.ambient = directionalLight.ambient;
	directionalLightVSOut.diffuse = directionalLight.diffuse;
	directionalLightVSOut.specular = directionalLight.specular;
	
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if (i >= numPointLights)
		{
			break;
		}
	    pointLightsVSOut[i].position = pointLights[i].position;
	    pointLightsVSOut[i].ambient = pointLights[i].ambient;
	    pointLightsVSOut[i].diffuse = pointLights[i].diffuse;
	    pointLightsVSOut[i].specular = pointLights[i].specular;
	    pointLightsVSOut[i].constant = pointLights[i].constant;
	    pointLightsVSOut[i].linear = pointLights[i].linear;
	    pointLightsVSOut[i].quadratic = pointLights[i].quadratic;
	}
	
	FragPos = vec3(model * vec4(aPos, 1.0));
	ViewPos = viewPos;
	
	
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	//adjustedFragPosLightSpace = FragPosLightSpace.xyz / FragPosLightSpace.w;
	
	
	if(texture_normal_set > 0)
	{
		FragPos = TBN * FragPos;
		ViewPos = TBN * ViewPos;
		directionalLightVSOut.direction = TBN * directionalLightVSOut.direction;
		for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
		{
			if (i >= numPointLights)
			{
				break;
			}
			pointLightsVSOut[i].position = TBN * pointLightsVSOut[i].position;
		}
	}
	
	//FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	directionalLightVSOut.direction = normalize(-directionalLightVSOut.direction);
	
	TexCoords = aTexCoords;
} 