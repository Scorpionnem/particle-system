#version 430

layout (local_size_x = 1) in;

layout(std430, binding = 0) buffer Pos {
	vec4 position[];
};

layout(std430, binding = 1) buffer Vel {
	vec4 velocity[];
};

uniform float GravityStrength = 2000.0;
uniform float deltaTime;
uniform float time;
uniform vec3 MAIN_ATTRACTOR;
uniform bool mouseClicked;
uniform unsigned int MAX_PARTICLES;

const float Damping = 0.99;
const float Softening = 2.0;

layout (binding = 2, offset = 0) uniform atomic_uint at_count;

void main()
{
    uint idx = gl_GlobalInvocationID.x;

    vec3 pos = position[idx].xyz;
    vec3 vel = velocity[idx].xyz;
    float   lifeTime = velocity[idx].w; // W value represents the lifetime of the particle

    if (lifeTime < 0)
    {
        position[idx] = vec4(0);
        velocity[idx].w = -1;
        return ;
    }
    else
    {
        atomicCounterIncrement(at_count);
        
        lifeTime -= 0.1 * deltaTime;

        if (mouseClicked)
        {
            vec3 toAttractor = MAIN_ATTRACTOR - pos;
            float dist = length(toAttractor);
            vec3 dir = normalize(toAttractor);
            float force = GravityStrength / (dist + Softening);
            vec3 acceleration = force * dir;

            vel += acceleration * deltaTime;
        }

        vel *= Damping;
        pos += vel * deltaTime;

        position[idx] = vec4(pos, 1.0);
        velocity[idx] = vec4(vel, lifeTime);
    }
}
