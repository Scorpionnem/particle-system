#version 460 core

layout(location = 0) in vec4 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;

uniform float MAX_PARTICLE_SIZE;

void main()
{
    vec4 worldPos = aPos;
    FragPos = worldPos.xyz;

    vec4 viewSpacePos = view * worldPos;
    float depth = -viewSpacePos.z;

    gl_Position = projection * viewSpacePos;

    // Now scale inversely with depth
    float size = clamp(MAX_PARTICLE_SIZE / (depth + 0.5), 2.0, MAX_PARTICLE_SIZE);
    gl_PointSize = size;
}
