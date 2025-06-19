#version 430

layout (local_size_x = 10) in;

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
    float   cubeSize = 20.0;

    return vec3(
        rand01(id * 3 + 0) * cubeSize - (cubeSize / 2),
        rand01(id * 3 + 1) * cubeSize - (cubeSize / 2),
        rand01(id * 3 + 2) * cubeSize - (cubeSize / 2)
    );
}

vec3 randomSphere(uint id)
{
    float radius = 10.0;

    float u = rand01(id * 3 + 0) * 2.0 - 1.0;
    float theta = rand01(id * 3 + 1) * 6.2831853;

    float sqrtOneMinusU2 = sqrt(1.0 - u * u);
    float x = sqrtOneMinusU2 * cos(theta);
    float y = sqrtOneMinusU2 * sin(theta);
    float z = u;

    vec3 dir = vec3(x, y, z);

    float r = pow(rand01(id * 3 + 2), 1.0/3.0) * radius;

    return dir * r;
}

uniform unsigned int u_Offset;
uniform unsigned int u_Count;
uniform bool shape;
uniform bool EMITTER;
uniform vec3 EMITTER_POS;

void main() {
    uint id = gl_GlobalInvocationID.x;
	if (id >= u_Count)
        return;

    uint idx = u_Offset + id;
    float   lifeTime = 100;

    vec3 pos;

    if (shape)
        pos = randomSphere(idx);
    else
        pos = randomCube(idx);

    if (EMITTER)
    {
        lifeTime = 1;
        pos += EMITTER_POS;
    }

    position[idx] = vec4(pos, 1.0);
    velocity[idx] = vec4(vec3(0.0), lifeTime);
}
