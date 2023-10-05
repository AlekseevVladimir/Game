#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
in vec3 ViewPos;
in mat3 TBN;


layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
  
#define MAX_POINT_LIGHTS 4

uniform int texture_normal_set;
uniform int texture_height_set;
struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
	sampler2D texture_normal;
	sampler2D texture_height;
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

vec3 getNormal();
vec2 ParallaxMapping();

void main()
{
    vec3 result = calculateDirLight();
    for (int i = 0; i < numPointLights; ++i) {
        result += calculatePointLight(pointLights[i]);
    }
    if (useSpotLight > 0){
        result += calculateSpotLight();
    }
	
    //vec3 norm = getNormal();
    //FragColor = vec4(norm, 1.0);
	FragColor = vec4(result, 1.0);
	// the constants are conventional for transforming from RGB to grayscale 
	//http://harmanani.github.io/classes/csc447/Notes/Lecture16.pdf
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	if (brightness > 1.0)
	{
		BrightColor = vec4(FragColor.rgb, 1.0);
	}
	//FragColor = vec4(farPlane);
	//FragColor = vec4(vec3(calculateOmnidirShadow(FragPos, pointLights[0].position)/25), 1.0);
	//FragColor = vec4(vec3(calculateOmnidirShadow(FragPos, pointLights[0].position)/farPlane), 1.0);
	//float depthValue = texture(shadowMap, TexCoords).r;
	//FragColor = vec4(vec3(depthValue), 1.0);
}  

vec3 calculateDirLight() {
	vec3 color = vec3(texture(material.texture_diffuse, ParallaxMapping()));
    vec3 ambient = directionalLight.ambient;

    vec3 norm = getNormal();
	vec3 lightDir = directionalLight.direction;
	if (texture_normal_set > 0)
	{
		lightDir = -(TBN * lightDir);
	}
	lightDir = normalize(-lightDir);
    
	//vec3 lightDir = normalize(-(TBN * directionalLight.direction));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuse * diff;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionalLight.specular * spec; 
    float shadow = calculateShadow(FragPosLightSpace, lightDir);
    return (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
}

vec3 calculatePointLight(PointLight light) {

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, ParallaxMapping()));

    vec3 norm = getNormal();
	vec3 lightPos = light.position;
	if (texture_normal_set > 0)
	{
		lightPos = TBN * lightPos;
	}
	
	vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, ParallaxMapping()));

    float specularStrength = 0.5;
    vec3 viewDir = normalize(ViewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, ParallaxMapping())); 
        
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	float shadow = calculateOmnidirShadow(FragPos, lightPos);
    //float shadow = calculateOmnidirShadow(FragPos, light.position);
	return (ambient  + (1.0 - shadow) * (diffuse + specular)) * attenuation;
}

vec3 calculateSpotLight() {


    vec3 ambient = spotLight.ambient * vec3(texture(material.texture_diffuse, ParallaxMapping()));

    vec3 norm = getNormal();
    
	vec3 lightDir = normalize(spotLight.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.texture_diffuse, ParallaxMapping()));

    float specularStrength = 0.5;
    vec3 viewDir = normalize(ViewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * vec3(texture(material.texture_specular, ParallaxMapping())); 
        
    float distance = length(spotLight.position - FragPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));

    
    float theta     = dot(lightDir, normalize(-spotLight.direction));
    float epsilon   = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0); 

    return (ambient + diffuse + specular) * attenuation * intensity;
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
	
	
    vec3 norm = getNormal();
	
	// TODO try using sollution from learnopengl as this creates shadow acne on the model
	float bias = max(0.003, 0.003 * (1 - dot(norm, lightDir)));
	
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
/*
float calculateOmnidirShadow(vec3 fragPos, vec3 lightPos)
{
	vec3 fragToLight = fragPos - lightPos;
	float closestDepth = texture(omnidirShadowMap, fragToLight).r;
	closestDepth *= farPlane;
	float currentDepth = length(fragToLight);
	float bias = 0.005f;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}
*/

float calculateOmnidirShadow(vec3 fragPos, vec3 lightPos)
{
	// TODO troll is wierdly dark with these shadows, need to check
	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	vec3 viewPos = ViewPos;
	
	if (texture_normal_set > 0)
	{
		fragPos = transpose(TBN) * fragPos;
		lightPos = transpose(TBN) * lightPos;
		viewPos = transpose(TBN) * viewPos;
	}
	
	vec3 fragToLight = fragPos - lightPos;
	float currentDepth = length(fragToLight);
	float viewDistance = length(viewPos - fragPos);
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

vec2 ParallaxMapping()
{ 
	if (texture_height_set > 0)
	{
		vec3 viewDir = normalize(ViewPos - FragPos);
		float height_scale = 0.1;
		
		const float numLayers = 10;
		// calculate the size of each layer
		float layerDepth = 1.0 / numLayers;
		// depth of current layer
		float currentLayerDepth = 0.0;
		// the amount to shift the texture coordinates per layer (from vector P)
		vec2 P = viewDir.xy * height_scale; 
		vec2 deltaTexCoords = P / numLayers;
		
		vec2  currentTexCoords     = TexCoords;
		float currentDepthMapValue = texture(material.texture_height, currentTexCoords).r;
		
		while(currentLayerDepth < currentDepthMapValue)
		{
			// shift texture coordinates along direction of P
			currentTexCoords -= deltaTexCoords;
			// get depthmap value at current texture coordinates
			currentDepthMapValue = texture(material.texture_height, currentTexCoords).r;  
			// get depth of next layer
			currentLayerDepth += layerDepth;  
		}
		
		return currentTexCoords;
	
	/*
		vec3 viewDir = normalize(ViewPos - FragPos);
		float height =  texture(material.texture_height, TexCoords).r;    
		float height_scale = 0.1;
		vec2 texCoords = viewDir.xy / viewDir.z * (height * height_scale);
		texCoords = TexCoords - texCoords;
		if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
		{
			discard;
		}
		return texCoords;
		*/
	}   
	else
	{
		return TexCoords;
	}
}

vec3 getNormal()
{
	vec3 norm;
	if(texture_normal_set > 0)
	{
		norm = texture(material.texture_normal, ParallaxMapping()).rgb;
		//probably remove tbn from here
		norm = normalize( (norm * 2.0 - 1.0));
	}
	else
	{
		norm = normalize(Normal);
	}
	return normalize(norm);
}