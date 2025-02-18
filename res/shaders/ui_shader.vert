#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection_matrix;

uniform int layer;
uniform float x_scale;
uniform float y_scale;
uniform float offset;
uniform float bezel;
uniform vec2 position;

void main()
{
    // variable to store new position of vertices
    vec2 updated_position;

    // detect which vertice we are looking at, and move it to the correct position
    if (aPos.x == 1 && aPos.y == 1) // top right vertice
    {
        updated_position = vec2(position.x + (x_scale / 2), position.y + (y_scale / 2));
    }
    if (aPos.x == 1 && aPos.y == -1) // bottom right vertice
    {
        updated_position = vec2(position.x + (x_scale / 2), position.y - (y_scale / 2));
    }
    // detect which vertice we are looking at, and move it to the correct position
    if (aPos.x == -1 && aPos.y == 1) // top left vertice
    {
        updated_position = vec2(position.x - (x_scale / 2), position.y + (y_scale / 2));
    }
    if (aPos.x == -1 && aPos.y == -1) // bottom left vertice
    {
        updated_position = vec2(position.x - (x_scale / 2), position.y - (y_scale / 2));
    }

    gl_Position = projection_matrix * vec4(updated_position, layer, 1.0);
}
