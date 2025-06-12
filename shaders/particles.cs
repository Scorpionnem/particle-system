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

uniform vec3 attractorPos;

void main()
{
    uint idx = gl_GlobalInvocationID.x;

    vec3 particlePos = position[idx].xyz;
    vec3 particleVel = velocity[idx].xyz;

    vec3 d = attractorPos - particlePos;
    float dist = length(d);
    vec3 force = (Gravity1 / dist) * normalize(d);

    if (dist > MaxDist)
        position[idx].xyz = vec3(0);
    else
    {
        vec3 a = force;
        position[idx] = vec4(particlePos + particleVel * DeltaT + 0.5 * a * DeltaT * DeltaT, 1.0);
        velocity[idx] = vec4(particleVel + a * DeltaT, 0.0);
    }
}
