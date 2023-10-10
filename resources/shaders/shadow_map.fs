#version 330 core

#define BIAS 0.005

void main() 
{
	//TODO idea: render front and back faces to separate textures
	gl_FragDepth = gl_FragCoord.z;
    gl_FragDepth += gl_FrontFacing ? BIAS : 0.0; // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
}
