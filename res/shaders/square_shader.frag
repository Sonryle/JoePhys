#version 330 core

in vec2 tex_coord;

uniform vec4 colour;
uniform sampler2D texture_atlas;

out vec4 FragColor;

void main()
{

    if (colour.r == -1.0f)	// if colour isnt set
    {
    	FragColor = texture(texture_atlas, tex_coord);
	return;
    }
    else
    {
    	FragColor = colour;
	return;
    }
} 
