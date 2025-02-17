#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 projection_matrix;

uniform int layer;
uniform float x_scale;
uniform float y_scale;
uniform vec2 position;
uniform vec2 top_left_tex_coord;
uniform vec2 top_right_tex_coord;
uniform vec2 bottom_left_tex_coord;
uniform vec2 bottom_right_tex_coord;

out vec2 tex_coord;

void main()
{
    //    We need to offset the vertices from aPos so that they form a square
    //    in the correct position and of the correct x and y scale.

    // variable to store new position of vertices
    vec2 updated_position;

    // identify which vertice we are looking at, and move it to the correct position
    if (aPos.x == 1 && aPos.y == 1) // top right vertice
        updated_position = vec2(position.x + (x_scale / 2), position.y + (y_scale / 2));
    if (aPos.x == 1 && aPos.y == -1) // bottom right vertice
        updated_position = vec2(position.x + (x_scale / 2), position.y - (y_scale / 2));
    if (aPos.x == -1 && aPos.y == 1) // top left vertice
        updated_position = vec2(position.x - (x_scale / 2), position.y + (y_scale / 2));
    if (aPos.x == -1 && aPos.y == -1) // bottom left vertice
        updated_position = vec2(position.x - (x_scale / 2), position.y - (y_scale / 2));

    //    We need to offset the tex_coord so that it matches with the texture
    //    coordinates that have been assigned to our square.

    // identify which texture coordinate we are looking at, and update it to be the proper tex_coord
    if (aTexCoord.x == 1.0f && aTexCoord.y == 1.0f) // top right tex_coord
    	tex_coord = top_right_tex_coord;
    if (aTexCoord.x == 0.0f && aTexCoord.y == 1.0f) // top left tex_coord
    	tex_coord = top_left_tex_coord;
    if (aTexCoord.x == 1.0f && aTexCoord.y == 0.0f) // bottom right tex_coord
    	tex_coord = bottom_right_tex_coord;
    if (aTexCoord.x == 0.0f && aTexCoord.y == 0.0f) // bottom left tex_coord
    	tex_coord = bottom_left_tex_coord;

    gl_Position = projection_matrix * vec4(updated_position, layer, 1.0);
}
