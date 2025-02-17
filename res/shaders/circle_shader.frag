#version 330 core
in vec2 frag_position;

uniform vec4 colour;
uniform float radius;

out vec4 FragColor;

void main()
{
    // Home made anti-aliasing
    // -----------------------

    // distance of our current fragment from center
    float dist = length(frag_position); // Distance from center

    // How many pixels around the edge should be affected
    int edge_pixel_thickness = 3;	

    float edge_of_circle = (radius - edge_pixel_thickness) / radius;

    float alpha = smoothstep(1.0, edge_of_circle, dist);

    FragColor = vec4(colour.rgb, colour.a * alpha);
} 
