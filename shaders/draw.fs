#version 460 core

out vec4 FragColor;
in vec3 FragPos;

uniform vec3 MAIN_ATTRACTOR;

uniform vec3 NEAR_COLOR;
uniform vec3 FAR_COLOR;

uniform float GRADIENT_SCALE;

uniform int PARTICLE_SHAPE;

void main() {
    if (PARTICLE_SHAPE == 1 && dot(gl_PointCoord - 0.5 , gl_PointCoord - 0.5) > 0.25) 
        discard;

    float distance = length(FragPos - MAIN_ATTRACTOR);
    float normalized = clamp(distance / GRADIENT_SCALE, 0.0, 1.0);

    FragColor = vec4(mix(NEAR_COLOR, FAR_COLOR, normalized), 1.0);
}
