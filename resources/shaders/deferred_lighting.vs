#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aBiTangent;

out vec2 TexCoords;
//out vec4 FragPosLightSpace;
//out vec3 FragPos;

//uniform mat4 lightSpaceMatrix;

void main()
{
    TexCoords = aTexCoords;
	//FragPos = texture(gPosition, TexCoords).rgb;
    gl_Position = vec4(aPos, 1.0);
	//FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}