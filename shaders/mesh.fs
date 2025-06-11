#version 330 core

in vec3 FragColor;
in vec3 FragPos;
in vec3 FragNormal;
in vec3 FragAbsNormal;

out vec4 outColor;

in vec2 texCoord;

uniform mat4 model;


uniform sampler2D tex0;

uniform vec3 lightPos;
uniform float ambientStrength;

uniform float time;

uniform vec3 viewPos;

void main()
{
	//Gets normals values from normal map
	//Gets texture's colors
	vec4 textureColor = texture(tex0, texCoord);
	vec4 finalTexture = textureColor;
	
	//Gets untextured colors
	//Discards magenta pixels to have transparency
	if (textureColor.r == 1 && textureColor.g == 0 && textureColor.b == 1)
        discard ;

	//Calculates all the phong lighting
	vec3 lightColor = vec3(0.7);
	vec3 ambient = 0.5 * lightColor;
	float specularStrength = 0.5;

	//Diffuse light
	vec3 norm = normalize(FragNormal);
	vec3 lightDir = normalize(viewPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//Specular light
	vec3 result = (ambient + diffuse);

	outColor = vec4(result, 1.0) * (finalTexture);
}
