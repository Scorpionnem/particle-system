#version 460 core

out vec4 FragColor;
in vec3 FragPos;

void main() {
    float distance = length(FragPos - vec3(0, 0, 0));

    // Normalize the distance if needed
    float normalized = clamp(distance / 250, 0.0, 1.0);

    // Example: grayscale color
    FragColor = vec4(vec3(1 - normalized), 1.0);

    // vec3 nearColor = vec3(0.2, 0.8, 1.0);
    // vec3 farColor  = vec3(1.0, 0.1, 0.1);
    // FragColor = vec4(mix(nearColor, farColor, normalized), 1.0);
}
