#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aBiTangent;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int texture_normal_set;


out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out mat3 TBN;


void main()
{
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	// to prevent normal distortions during transforming (scaling i.e.)
	Normal = mat3(transpose(inverse(model))) * aNormal;
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	//T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(vec3(model * vec4(aBiTangent, 0.0)));//cross(N, T);
	
	TBN = mat3(T, B, N);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	
	TexCoords = aTexCoords;
} 