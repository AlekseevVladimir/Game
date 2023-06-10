#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
out vec4 FragColor;
  
#define MAX_POINT_LIGHTS 4

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D emission;
    float shininess;
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
uniform float test1 = 0;
uniform float test2 = 0;
uniform float test3 = 0;

uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
  
uniform Material material;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float alpha;

uniform sampler2D shadowMap;
uniform samplerCube omnidirShadowMap;
 
vec3 calculateDirLight();
vec3 calculatePointLight(PointLight light);
vec3 calculateSpotLight();

float calculateShadow(vec4 fragPosLightSpace);
float calculateOmnidirShadow(vec3 fragPos, vec3 lightPos);
float farPlane;

void main()
{
    vec3 result = vec3(0.0);// = calculateDirLight();
    for (int i = 0; i < numPointLights; ++i) {
        result = calculatePointLight(pointLights[i]);
    }
    if (useSpotLight > 0){
        result += calculateSpotLight();
    }
    FragColor = vec4(result, 1.0);
	//FragColor = vec4(vec3(calculateOmnidirShadow(FragPos, pointLights[0].position)/farPlane), 1.0);
	//float depthValue = texture(shadowMap, TexCoords).r;
	//FragColor = vec4(vec3(depthValue), 1.0);
}  

vec3 calculateDirLight() {
	vec3 color = vec3(texture(material.texture_diffuse, TexCoords));
    vec3 ambient = directionalLight.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-directionalLight.direction);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuse * diff;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionalLight.specular * spec; 
    float shadow = calculateShadow(FragPosLightSpace);
	//return vec3((1.0 - shadow));
    return (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
}

vec3 calculatePointLight(PointLight light) {

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords)); 
        
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	float shadow = calculateOmnidirShadow(FragPos, light.position);
    return (ambient  + (1.0 - shadow) * (diffuse + specular)) * attenuation;
}

vec3 calculateSpotLight() {


    vec3 ambient = spotLight.ambient * vec3(texture(material.texture_diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(spotLight.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos-FragPos);
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

float calculateShadow(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.z > 1.0)
	{
		return 0.0;
	}
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = 0.005;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	//return 0.0;
	return shadow;
}

float calculateOmnidirShadow(vec3 fragPos, vec3 lightPos)
{
	//return 0.0;
	vec3 fragToLight = fragPos - lightPos;
	float closestDepth = texture(omnidirShadowMap, fragToLight).r;
	//
	return closestDepth;
	//
	closestDepth *= farPlane;
	float currentDepth = length(fragToLight);
	float bias = 0.5f;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}