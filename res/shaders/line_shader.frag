#version 330 core
in vec2 frag_position;

uniform vec4 colour;
uniform float thickness;

out vec4 FragColor;

void main()
{
    // Home made anti-aliasing
    // -----------------------

    //   This method only affects the left and right edges
    //   of the line (the top and bottom remain aliased)

    //	 keep in mind that frag_position is just aPos from
    // 	 the vertex shader, and so it has not been manipulated
    // 	 into the size or rotation of our line

    // distance from center of line (on x axis)
    float dist = (frag_position.x > 0)? frag_position.x : -frag_position.x;

    int edge_pixel_thickness = 3;	// the amount of pixels at the edge that will be faded away
    float edge_of_line = ((thickness / 2) - edge_pixel_thickness) / (thickness / 2);

    float alpha = smoothstep(1.0, edge_of_line, dist);

    FragColor = vec4(colour.rgb, colour.a * alpha);
} 
