#version 430

layout (local_size_x = 10, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer Pos {
	vec4 position[];
};

layout(std430, binding = 1) buffer Vel {
	vec4 velocity[];
};

uniform float GravityStrength = 1000.0;
uniform float deltaTime;
uniform float time;
uniform vec3 MAIN_ATTRACTOR;
uniform vec3 SECONDARY_ATTRACTOR;
uniform bool mouseClicked;
uniform unsigned int MAX_PARTICLES;

const float Damping = 0.99;
const float Softening = 2.0;

layout (binding = 2, offset = 0) uniform atomic_uint live_count;

vec3   calcVelocityToAttractor(vec3 particlePos, vec3 attractor, float gravity)
{
    vec3   res;
    vec3 toAttractor = attractor - particlePos;
    float dist = length(toAttractor);
    vec3 dir = normalize(toAttractor);
    float force = gravity / (dist + Softening);
    vec3 acceleration = force * dir;
    
    res = acceleration * deltaTime;
    return (res);
}

void main()
{
    uint idx = gl_GlobalInvocationID.x;

    vec3 pos = position[idx].xyz;
    vec3 vel = velocity[idx].xyz;
    float   lifeTime = velocity[idx].w; // W value represents the lifetime of the particle

    if (lifeTime < 0 && lifeTime < 100)
    {
        position[idx] = vec4(0);
        velocity[idx].w = -1;
        return ;
    }
    else
    {
        atomicCounterIncrement(live_count);
        
        if (lifeTime < 100)
            lifeTime -= 0.1 * deltaTime;

        if (mouseClicked)
        {
            vel += calcVelocityToAttractor(pos, MAIN_ATTRACTOR, GravityStrength);
            vel += calcVelocityToAttractor(pos, SECONDARY_ATTRACTOR, GravityStrength);
        }

        vel -= (vel * Damping) * deltaTime;
        pos += vel * deltaTime;

        position[idx] = vec4(pos, 1.0);
        velocity[idx] = vec4(vel, lifeTime);
    }
}
