#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexPos;

out vec3 FragColor;
out vec3 FragPos;
out vec3 FragNormal;
out vec3 FragAbsNormal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

// Controls how much the world curves — tweak as needed
uniform float curveAmount = 190.0;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);

	// Distance to origin in XZ plane
	float dist = length(vec2(viewPos.z, worldPos.z));
	worldPos.y -= (dist * dist) / curveAmount;
	dist = length(vec2(viewPos.x, worldPos.x));
	worldPos.y -= (dist * dist) / curveAmount;

    // Apply model/view/projection to curved position
    gl_Position = projection * view * worldPos;

    FragColor = aColor;
    FragAbsNormal = abs(aNormal);

    FragPos = vec3(worldPos);
    FragNormal = mat3(transpose(inverse(model))) * aNormal;
    texCoord = aTexPos;
}
