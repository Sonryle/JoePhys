#version 330 core
const vec4 colour = vec4(1.0, 1.0, 0.0, 1.0);
const vec4 bezelColour = vec4(1.0, 0.0, 0.0, 1.0);
const float bezel = 30.0;
const float bezelThickness = 10.0;
uniform float x_scale;
uniform float y_scale;
uniform vec2 position;

vec2 scale = vec2(x_scale, y_scale);
in vec2 texCoord;

float SDF(vec2 p, vec2 b, float r)
{
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

void main()
{
    vec2 pos = scale * texCoord;

    float distance = SDF(pos-scale/2.0, scale/2.0 - bezelThickness/2.0-1.0, bezel);
    float blendFactor = smoothstep(-1.0, 1.0, distance);

    vec4 fromColour = bezelColour;
    vec4 toColour = vec4(0.0, 0.0, 0.0, 0.0);
    gl_FragColor = mix(fromColour, toColour, blendFactor);
}