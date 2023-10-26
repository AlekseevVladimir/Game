#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aBiTangent;

uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 LocalPos;

void main()
{
    TexCoords = aTexCoords;
	LocalPos = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);;
	
	
}