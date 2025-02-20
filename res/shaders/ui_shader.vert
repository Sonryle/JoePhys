#version 330 core
layout (location = 0) in vec2 aPos; // Changed to vec2 since we are only using 2D positions
layout (location = 1) in vec2 aTexCoord; // Texture coordinates

out vec2 texCoord; // output texture coordinates to fragment shader

uniform mat4 projection_matrix;
uniform int layer;
uniform float x_scale;
uniform float y_scale;
uniform vec2 position;

void main()
{
    // store new position of vertices
    vec2 updated_position;

    // Detect which vertex we are looking at, and move it to the correct position
    if (aPos.x == 1 && aPos.y == 1) // Top right vertex
    {
        updated_position = vec2(position.x + (x_scale / 2), position.y + (y_scale / 2));
    }
    else if (aPos.x == 1 && aPos.y == -1) // Bottom right vertex
    {
        updated_position = vec2(position.x + (x_scale / 2), position.y - (y_scale / 2));
    }
    else if (aPos.x == -1 && aPos.y == 1) // Top left vertex
    {
        updated_position = vec2(position.x - (x_scale / 2), position.y + (y_scale / 2));
    }
    else if (aPos.x == -1 && aPos.y == -1) // Bottom left vertex
    {
        updated_position = vec2(position.x - (x_scale / 2), position.y - (y_scale / 2));
    }

    // give texture coordinates to fragment shader
    texCoord = aTexCoord;

    // refresh viewport with new position
    gl_Position = projection_matrix * vec4(updated_position, layer, 1.0);
}