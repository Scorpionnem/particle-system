#version 430

layout (local_size_x = 1) in;

layout(std430, binding = 0) buffer Pos {
	vec4 position[];
};

layout(std430, binding = 1) buffer Vel {
	vec4 velocity[];
};

uint hash(uint x)
{
    x ^= x >> 16;
    x *= 0x7feb352d;
    x ^= x >> 15;
    x *= 0x846ca68b;
    x ^= x >> 16;
    return x;
}

float rand01(uint seed)
{
    return float(hash(seed) & 0xFFFFu) / 65535.0;
}

vec3 randomCube(uint id)
{
    return vec3(
        rand01(id * 3 + 0) * 20.0 - 1.0,
        rand01(id * 3 + 1) * 20.0 - 1.0,
        rand01(id * 3 + 2) * 20.0 - 1.0
    );
}

uniform unsigned int oldParticleCount;

void main() {
    uint id = gl_GlobalInvocationID.x;
	if (id <= oldParticleCount)
		return;

    vec3 pos = randomCube(id);

    position[id] = vec4(pos, 1.0);
    velocity[id] = vec4(0.0);
}
