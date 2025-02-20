#version 330 core
uniform vec4 colour;
uniform vec4 bezelColour;
uniform float bezel;
uniform float bezelThickness;
uniform float x_scale;
uniform float y_scale;
uniform vec2 position;

vec2 scale = vec2(x_scale, y_scale);
in vec2 texCoord;

float SDF(vec2 p, vec2 b, float r)
{
    // signed distance field, not even going to bother explaining this
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

void main()
{
    // scale texture coordinates to match the aspect ratio of the UI element
    vec2 pos = scale * texCoord;

    // calculate signed distance field value for the current pixel
    float distance = SDF(pos-scale / 2.0, scale / 2.0 - bezelThickness / 2.0 - 1.0, bezel);
    float blendFactor = smoothstep(-1.0, 1.0, abs(distance) - bezelThickness / 2.0);

    // calculate colours for the bezel and the main area
    vec4 fromColour = bezelColour;
    vec4 toColour = (distance < 0.0) ? colour : vec4(0.0);

    // blend colours together based on calculated blend factor, resulting in a sharp transition
    gl_FragColor = mix(fromColour, toColour, blendFactor);
}