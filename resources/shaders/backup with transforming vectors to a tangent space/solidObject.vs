#version 330 core

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


out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec4 FragPosLightSpace;
out vec3 ViewPos;
out mat3 TBN;


void main()
{
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	// TODO check out why is this like this
	Normal = mat3(transpose(inverse(model))) * aNormal;
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	//T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(vec3(model * vec4(aBiTangent, 0.0)));//cross(N, T);
	
	TBN = transpose(mat3(T, B, N));
    //TBN = mat3(
	//vec3(1, 0, 0),
	//vec3(0, 1, 0),
	//vec3(0, 0, 1)
	//);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	ViewPos = viewPos;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	if(texture_normal_set > 0)
	{
		FragPos = TBN * FragPos;
		ViewPos = TBN * ViewPos;
	}
	TexCoords = aTexCoords;
} 