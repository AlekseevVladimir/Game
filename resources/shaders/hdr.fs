#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;
uniform float exposure;
uniform sampler2D hdrBuffer;

void main()
{          
	float gamma = 1.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
	mapped = pow(mapped, vec3(1.0 / gamma));
	FragColor = vec4(mapped, 1.0);
}  