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
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

void main()
{
    /*vec2 pos = scale * texCoord;

    float distance = SDF(pos-scale/2.0, scale/2.0 - bezelThickness/2.0-1.0, bezel);
    float blendFactor = smoothstep(-1.0, 1.0, distance);

    vec4 fromColour = bezelColour;
    vec4 toColour = vec4(0.0, 0.0, 0.0, 0.0);

    // Debugging colors
    if (distance < 0.0) {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red for inside the bezel
    } else if (distance < 1.0) {
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Green for the bezel
    } else {
        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue for outside the bezel
    }

    // Original blending
    // gl_FragColor = mix(fromColour, toColour, blendFactor);*/

    gl_FragColor = colour;
}