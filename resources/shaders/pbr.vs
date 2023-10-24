#version 330 core


layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inTangent;
layout (location = 3) in vec2 inTexCoords;
layout (location = 4) in vec3 inBiTangent;

out vec2 aTexCoords;
out vec3 aWorldPos;
out vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    aTexCoords = inTexCoords;
	aWorldPos = vec3(model * vec4(inPos, 1.0));
	aNormal = mat3(transpose(inverse(model))) * inNormal;
    gl_Position = projection * view * model * vec4(inPos, 1.0);
}