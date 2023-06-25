#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
in vec3 ViewPos;
in mat3 TBN;

out vec4 FragColor;
  
#define MAX_POINT_LIGHTS 4

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
	sampler2D texture_normal;
    sampler2D emission;
    float shininess;
}; 

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

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;

uniform int useSpotLight = 0;

uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
  
uniform Material material;

uniform sampler2D shadowMap;
uniform samplerCube omnidirShadowMap;
 
vec3 calculateDirLight();
vec3 calculatePointLight(PointLight light);
vec3 calculateSpotLight();

float calculateShadow(vec4 fragPosLightSpace, vec3 lightDir);
float calculateOmnidirShadow(vec3 fragPos, vec3 lightPos);

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

void main()
{
    vec3 result = calculateDirLight();
    for (int i = 0; i < numPointLights; ++i) {
        result += calculatePointLight(pointLights[i]);
    }
    if (useSpotLight > 0){
        result += calculateSpotLight();
    }
    FragColor = vec4(result, 1.0);
	//FragColor = vec4(farPlane);
	//FragColor = vec4(vec3(calculateOmnidirShadow(FragPos, pointLights[0].position)/25), 1.0);
	//FragColor = vec4(vec3(calculateOmnidirShadow(FragPos, pointLights[0].position)/farPlane), 1.0);
	//float depthValue = texture(shadowMap, TexCoords).r;
	//FragColor = vec4(vec3(depthValue), 1.0);
}  

vec3 calculateDirLight() {
	vec3 color = vec3(texture(material.texture_diffuse, TexCoords));
    vec3 ambient = directionalLight.ambient;

    //vec3 norm = normalize(Normal);
    
    vec3 norm = texture(material.texture_normal, TexCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	vec3 lightDir = TBN * normalize(-directionalLight.direction);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuse * diff;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(ViewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionalLight.specular * spec; 
    float shadow = calculateShadow(FragPosLightSpace, lightDir);
    return (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
}

vec3 calculatePointLight(PointLight light) {

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));

    //vec3 norm = normalize(Normal);
    vec3 norm = texture(material.texture_normal, TexCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	
	vec3 lightDir = normalize(TBN * light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));

    float specularStrength = 0.5;
    vec3 viewDir = normalize(ViewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords)); 
        
    float distance = length(TBN * light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	float shadow = calculateOmnidirShadow(FragPos, TBN * light.position);
    return (ambient  + (1.0 - shadow) * (diffuse + specular)) * attenuation;
}

vec3 calculateSpotLight() {


    vec3 ambient = spotLight.ambient * vec3(texture(material.texture_diffuse, TexCoords));

    //vec3 norm = normalize(Normal);
    
    vec3 norm = texture(material.texture_normal, TexCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	vec3 lightDir = normalize(spotLight.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));

    float specularStrength = 0.5;
    vec3 viewDir = normalize(ViewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * vec3(texture(material.texture_specular, TexCoords)); 
        
    float distance = length(spotLight.position - FragPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));

    
    float theta     = dot(lightDir, normalize(-spotLight.direction));
    float epsilon   = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0); 

    return (ambient + diffuse + specular) * attenuation * intensity;
}

float calculateShadow(vec4 fragPosLightSpace, vec3 lightDir)
{
	return 0.0;
	vec3 projCoords = TBN * (fragPosLightSpace.xyz / fragPosLightSpace.w);
	projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.z > 1.0)
	{
		return 0.0;
	}
	float currentDepth = projCoords.z;
	
	
    vec3 norm = texture(material.texture_normal, TexCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	
	// new bias or pcf broke cube's shadow
	float bias = max(0.005, 0.1 * (1 - dot(norm, lightDir)));
	
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
	
	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow / 9.0;
}

float calculateOmnidirShadow(vec3 fragPos, vec3 lightPos)
{
	return 0.0;
	vec3 fragToLight = fragPos - lightPos;
	float closestDepth = texture(omnidirShadowMap, fragToLight).r;
	closestDepth *= farPlane;
	float currentDepth = length(fragToLight);
	float bias = 0.005f;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

/*
float calculateOmnidirShadow(vec3 fragPos, vec3 lightPos)
{
	float shadow = 0.0;
	
	float bias = 0.15;
	int samples = 20;
	vec3 fragToLight = fragPos - lightPos;
	float currentDepth = length(fragToLight);
	float viewDistance = length(ViewPos - fragPos);
	float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;
	for (int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(
			omnidirShadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= farPlane;
		if (currentDepth - bias > closestDepth)
		{
			shadow += 1.0;
		}
	}
	return shadow / float(samples);
}
*/