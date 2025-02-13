#version 330 core
in vec2 frag_position;

uniform vec4 colour;

out vec4 FragColor;

void main()
{
    // if fragment falls outside of circle, then discard
    if (sqrt(frag_position.x * frag_position.x + frag_position.y * frag_position.y) > 1)
        discard;

    FragColor = colour;
} 