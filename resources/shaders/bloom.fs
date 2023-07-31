#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float fragmentWight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{          
	vec2 texOffset = 1.0 / textureSize(image, 0);
	vec3 result = texture(image, TexCoords).rgb * fragmentWight[0];
	vec2 offsetVector;
	if (horizontal)
	{
		offsetVector = vec2(texOffset.x, 0.0);
	}
	else
	{
		offsetVector = vec2(0.0, texOffset.y);
	}
	for (int i = 1; i < 5; ++i)
	{
		result += texture(image, TexCoords + offsetVector * i).rgb * fragmentWight[i];
		result += texture(image, TexCoords - offsetVector * i).rgb * fragmentWight[i];
	}
	FragColor = vec4(result, 1.0);
}  