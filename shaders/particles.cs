#version 430

layout (local_size_x = 1) in;

layout(std430, binding = 0) buffer Pos {
	vec4 position[];
};

layout(std430, binding = 1) buffer Vel {
	vec4 velocity[];
};

uniform float Gravity1 = 1000.0;

const float ParticleMass = 0.1;
const float ParticleInvMass = 1.0 / ParticleMass;

uniform float MaxDist = 2000.0;

const float DeltaT = 0.0005;

uniform float time;

void main() {
    uint idx = gl_GlobalInvocationID.x;
    vec3 BlackHolePos1 = vec3(0,0,0);

    vec3 p = position[idx].xyz;

    BlackHolePos1.xyz = vec3(sin(time), cos(time), sin(time));
    vec3 d = BlackHolePos1 - p;
    float dist = length(d);
    vec3 force = (Gravity1 / dist) * normalize(d);

    if (dist > MaxDist)
    {
        position[idx].xyz = BlackHolePos1;
    }
    else
    {
        vec3 a = force * ParticleInvMass;
        position[idx] = vec4(p + velocity[idx].xyz * DeltaT + 0.5 * a * DeltaT * DeltaT, 1.0);
        velocity[idx] = vec4( velocity[idx].xyz + a * DeltaT, 0.0);
    }
}
