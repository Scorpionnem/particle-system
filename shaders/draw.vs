#version 460 core

layout(location = 0) in vec4 position;

uniform mat4 projection;
uniform mat4 view;

out	vec3 FragPos;

void main() {
    gl_PointSize = 1.0; // You can also set this from CPU
    gl_Position = projection * view * position;
	FragPos = position.xyz;
}
