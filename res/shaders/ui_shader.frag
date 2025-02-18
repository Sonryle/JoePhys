#version 330 core
uniform vec4 colour;
uniform vec4 bezelColour;
uniform float bezel;
uniform float bezelThickness;
uniform float x_scale;
uniform float y_scale;
uniform vec2 position;

in vec2 texCoord;

vec2 scale = vec2(x_scale, y_scale);

float SDF(vec2 p, vec2 b, float r)
{
    vec2 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - r;
}

void main()
{
    vec2 pos = scale * (texCoord - 0.5) + position;

    float distance = SDF(pos, scale / 2.0 - bezelThickness / 2.0, bezel);
    float blendFactor = smoothstep(-bezelThickness / 2.0, bezelThickness / 2.0, distance);

    vec4 fromColour = bezelColour;
    vec4 toColour = (distance < 0.0) ? colour : vec4(0.0);

    gl_FragColor = mix(fromColour, toColour, blendFactor);
}