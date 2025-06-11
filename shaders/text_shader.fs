#version 330 core
out vec4 FragColor;

uniform sampler2D tex0;

in vec2 fragPos;
in vec4 clip;

uniform float time;
uniform float SCREEN_WIDTH;
uniform float SCREEN_HEIGHT;
uniform bool rainbow;
uniform vec3 color;

void main() {
    vec4 texColor = texture(tex0, fragPos);
    if (texColor.r == 0, texColor.g == 0 && texColor.b == 0)
        discard ;
    if (rainbow)
    {
        vec3 ndc = vec3(clip) / clip.w;

        float screenX = (ndc.x / 2.0f + 0.5f) * SCREEN_WIDTH;
        float screenY = (ndc.y / 2.0f + 0.5f) * SCREEN_HEIGHT;

        float speed = time * 4.0;
        speed = time * 20.0;
        float offset = (screenX + sin(screenY / 8.0 + time / 2) / 20) / 20;

        float red   = 0.5 + sin(offset + speed + 0.0) / 2;
        float green = 0.5 + sin(offset + speed + 2.0) / 2;
        float blue  = 0.5 + sin(offset + speed + 4.0) / 2;

        FragColor = vec4(red, green, blue, 1.0);
    }
    else
        FragColor = vec4(color, 1.0);
}
