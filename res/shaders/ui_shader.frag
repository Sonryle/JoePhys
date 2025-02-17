#version 330 core
uniform vec4 colour;
uniform vec4 bezelColour;
uniform float bezel;
uniform float x_scale;
uniform float y_scale;
uniform vec2 position;

out vec4 FragColor;
in vec2 fragCoord;

void main()
{
    // Get the fragment coordinates
    vec2 fragCoord = gl_FragCoord.xy;
    // Normalize the fragment coordinates to range from -0.5 to 0.5
    vec2 normalizedCoord = (fragCoord - position) / vec2(x_scale, y_scale);

    // Calculate the distance from the center of the square
    float distanceFromCenter = length(normalizedCoord);

    // If the distance is within the bezel range
    if (distanceFromCenter > 0.5 - bezel && distanceFromCenter <= 0.5)
    {
        // Bezel area
        FragColor = bezelColour;
    }
    else
    {
        // Square area
        FragColor = colour;
    }
}