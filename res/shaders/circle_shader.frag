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
    int aa_edge_pixel_thickness = 1;
    int outline_edge_pixel_thickness = 10;

    float aa_edge_of_circle = (radius - aa_edge_pixel_thickness) / radius;
    float outline_edge_of_circle = (radius - outline_edge_pixel_thickness) / radius;

    float alpha = smoothstep(1.0, aa_edge_of_circle, dist);
    float darkness = smoothstep(1.0, outline_edge_of_circle, dist);
    if (length(frag_position) >= outline_edge_of_circle)
    	darkness = 0.3f;

    FragColor = vec4(colour.rgb * darkness, colour.a * alpha);
} 
