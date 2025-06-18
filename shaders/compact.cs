#version 430

layout(local_size_x = 1) in;

// Input buffers (all particles)
layout(std430, binding = 0) buffer PosIn {
    vec4 in_positions[];
};

layout(std430, binding = 1) buffer VelIn {
    vec4 in_velocities[];
};

// Temporary output buffers (max _usedParticles in size)
layout(std430, binding = 2) buffer PosOut {
    vec4 out_positions[];
};

layout(std430, binding = 3) buffer VelOut {
    vec4 out_velocities[];
};

// Atomic counter to count how many particles were copied
layout(binding = 4, offset = 0) uniform atomic_uint compactedCount;

void main()
{
    uint idx = gl_GlobalInvocationID.x;

    vec4 vel = in_velocities[idx];

    // Skip dead particles
    if (vel.w < 0.0)
        return;

    // Get a write index for live particle
    uint writeIdx = atomicCounterIncrement(compactedCount);

    // Write live particle data to compacted buffers
    out_positions[writeIdx] = in_positions[idx];
    out_velocities[writeIdx] = vel;
}
