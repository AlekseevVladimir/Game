#version 330 core

//in vec3 FragPos;
in vec2 TexCoords;
//in vec4 FragPosLightSpace;

out vec4 FragColor;

  
#define MAX_POINT_LIGHTS 4

uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecShine;
uniform sampler2D gPosition;


uniform float farPlane;
struct PointLight {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
 
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 lightSpaceMatrix;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;

uniform int useSpotLight = 0;

uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;

uniform sampler2D shadowMap;
uniform samplerCube omnidirShadowMap;

uniform vec3 viewPos;

vec3 calculateDirLight();
float calculateShadow(vec4 fragPosLightSpace, vec3 lightDir);

void main()
{
	vec3 result = calculateDirLight();
	//FragColor = vec4(vec3(texture(gPosition, TexCoords)), 1.0);
	FragColor = vec4(result, 1.0);
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}  


vec3 calculateDirLight() {
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 color = vec3(texture(gAlbedo, TexCoords).rgb);
    vec3 ambient = directionalLight.ambient;

    vec3 norm = vec3(texture(gNormal, TexCoords).rgb);
	vec3 lightDir = directionalLight.direction;

	lightDir = normalize(-lightDir);
    
	//vec3 lightDir = normalize(-(directionalLight.direction));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuse * diff;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), texture(gSpecShine, TexCoords).a);
    vec3 specular = directionalLight.specular * spec; 
	vec4 FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    float shadow = calculateShadow(FragPosLightSpace, lightDir);
    return (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	//return vec3(shadow, shadow, shadow);
	//return FragPosLightSpace.xyz;
}

float calculateShadow(vec4 fragPosLightSpace, vec3 lightDir)
{
	vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w);
	projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.z > 1.0)
	{
		return 0.0;
	}
	float currentDepth = projCoords.z;
	
	
    vec3 norm = vec3(texture(gNormal, TexCoords).rgb);
	
	// TODO try using sollution from learnopengl as this creates shadow acne on the model
	float bias = 0;//max(0.003, 0.003 * (1 - dot(norm, lightDir)));
	
	float shadow = 0.0;
	// TODO READ ABOUT TEXTURE SIZE, LOD, MIP MAPS
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	return shadow / 9.0;
}