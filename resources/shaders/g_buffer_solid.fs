
#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;


layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gSpecShine;
  
#define MAX_POINT_LIGHTS 4

uniform vec3 viewPos;
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


uniform Material material;


vec3 getNormal();
vec2 ParallaxMapping();

void main()
{

	gNormal.rgb = getNormal();
	gNormal.a = 1.0;
	gPosition.rgb = FragPos;
	gPosition.a = 1.0;
	gAlbedo.rgb = vec3(texture(material.texture_diffuse, ParallaxMapping()));
	gAlbedo.a = 1.0;
	//gSpecShine.rgb = vec3(texture(material.texture_specular, ParallaxMapping()));
	//gSpecShine.a = material.shininess;

}  

vec2 ParallaxMapping()
{ 
	if (texture_height_set > 0)
	{
		vec3 viewDir = normalize(viewPos - FragPos);
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
		vec3 viewDir = normalize(viewPos - FragPos);
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
		norm = normalize( TBN * (norm * 2.0 - 1.0));
	}
	else
	{
		norm = normalize(Normal);
	}
	return normalize(norm);
}