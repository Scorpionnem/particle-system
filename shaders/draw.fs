#version 460 core

out vec4 FragColor;
in vec3 FragPos;
in vec4 FragVel;

uniform vec3 MOUSE_POS;

uniform vec3 NEAR_COLOR;
uniform vec3 FAR_COLOR;

uniform float GRADIENT_SCALE;

uniform int PARTICLE_SHAPE;

uniform bool    VELOCITY_COLOR;

void main() {
    if (PARTICLE_SHAPE == 1 && dot(gl_PointCoord - 0.5 , gl_PointCoord - 0.5) > 0.25) 
        discard;

    if (FragVel.w < 0)
        discard ;

    float distance = length(FragPos - MOUSE_POS);
    float normalized = clamp(distance / GRADIENT_SCALE, 0.0, 1.0);

    if (VELOCITY_COLOR)
        FragColor = vec4(normalize(abs(FragVel.xyz)), FragVel.w);
    else
        FragColor = vec4(mix(NEAR_COLOR, FAR_COLOR, normalized), FragVel.w);
}
