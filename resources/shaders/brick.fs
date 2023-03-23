#version 330 core

in vec2 TexCoords;

out vec4 FragColor;
uniform vec3 viewPos;
uniform sampler2D image;
uniform vec3 color;
 
void main()
{
    FragColor = texture(image, TexCoords);
}