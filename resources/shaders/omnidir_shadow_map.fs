#version 330 core

#define BIAS 0.05

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
	// get distance between fragment and light source
	float lightDistance = length(FragPos.xyz - lightPos);
	
	// map to [0;1] range by dividing by farPlane
	lightDistance = lightDistance / farPlane;
	
	gl_FragDepth = lightDistance;
	
    gl_FragDepth += gl_FrontFacing ? BIAS : 0.0; //  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 
}