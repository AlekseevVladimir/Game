#version 330 core

#define MAX_POINT_LIGHTS 4

struct Material
{
	sampler2D texture_albedo;
	sampler2D texture_metallic;
	sampler2D texture_roughness;
	sampler2D texture_ao;
	sampler2D texture_normal;
};

struct PointLight
{
	vec3 position;
	vec3 color;
};

in vec2 aTexCoords;
in vec3 aWorldPos;
in vec3 aNormal;

out vec4 outFragColor;

uniform vec3 viewPos;

uniform Material uMaterial;

uniform PointLight uPointLights[MAX_POINT_LIGHTS];
uniform int uPointLightsNum;

vec3 fresnelSchlick(float cosTheta, vec3 F0);

float distributionGGX(vec3 N, vec3 H, float roughness);

float geometrySchlickGGX(float NdotV, float roughness);

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

vec3 getNormalFromMap();

const float PI = 3.14159265359;

void main()
{
	vec3 N = getNormalFromMap();
	vec3 V = normalize(viewPos - aWorldPos);
	vec3 albedo = texture(uMaterial.texture_albedo, aTexCoords).rgb;
	float metallic = texture(uMaterial.texture_metallic, aTexCoords).r;
	float roughness = texture(uMaterial.texture_roughness, aTexCoords).r;
	float ao = texture(uMaterial.texture_ao, aTexCoords).r;
	vec3 Lo = vec3(0.0);
	for (int i = 0; i < uPointLightsNum; i++)
	{
		vec3 L = normalize(uPointLights[i].position - aWorldPos);
		vec3 H = normalize(V + L);
		float distance = length(uPointLights[i].position - aWorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = uPointLights[i].color * attenuation;
		
		vec3 F0 = vec3(0.04);
		F0 = mix(F0, albedo, metallic);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
		
		float NDF = distributionGGX(N, H, roughness);
		float G = geometrySmith(N, V, L, roughness);
		
		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
		vec3 specular = numerator / denominator;
		
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		
		kD *= 1.0 - metallic;
		
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}
	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 
	outFragColor = vec4(color, 1.0);
	//outFragColor = vec4(metallic, metallic, metallic, metallic);
}


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;
	
	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	
	return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	
	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	
	return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(uMaterial.texture_normal, aTexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(aWorldPos);
    vec3 Q2  = dFdy(aWorldPos);
    vec2 st1 = dFdx(aTexCoords);
    vec2 st2 = dFdy(aTexCoords);

    vec3 N   = normalize(aNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}