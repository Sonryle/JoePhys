#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection_matrix;
uniform vec2 position;
uniform float radius;
uniform int layer;

out vec2 frag_position;

void main()
{
    vec2 updated_position;

    // detect which vertice we are looking at, and move it to the correct position
    if (aPos.x == 1 && aPos.y == 1) // top right vertice
    {
        updated_position.x = position.x + radius;
        updated_position.y = position.y + radius;
    }
    if (aPos.x == 1 && aPos.y == -1) // bottom right vertice
    {
        updated_position.x = position.x + radius;
        updated_position.y = position.y - radius;
    }
    if (aPos.x == -1 && aPos.y == 1) // top left vertice
    {
        updated_position.x = position.x - radius;
        updated_position.y = position.y + radius;
    }
    if (aPos.x == -1 && aPos.y == -1) // bottom left vertice
    {
        updated_position.x = position.x - radius;
        updated_position.y = position.y - radius;
    }

    gl_Position = projection_matrix * vec4(updated_position, layer, 1.0);
    frag_position = vec2(aPos.x, aPos.y);
}
