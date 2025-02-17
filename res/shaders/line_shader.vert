#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection_matrix;
uniform vec2 start_position;
uniform vec2 end_position;
uniform float thickness;
uniform int layer;

out vec2 frag_position;

void main()
{
    // variable to store new position of vertices
    vec2 updated_position;

    // get angle between the start and end points
    vec2 angle_between_points = start_position - end_position;

    // get the angle PERPENDICULAR to the angle between the start and end points
    vec2 perpendicular_angle = vec2(angle_between_points.y, -angle_between_points.x);

    // normalise perpendicular angle
    perpendicular_angle = normalize(perpendicular_angle);

    // detect which vertice we are looking at, and move it to the correct position
    if (aPos.x == 1 && aPos.y == 1) // top right vertice
    {
        updated_position = start_position + perpendicular_angle * (thickness / 2);
    }
    if (aPos.x == 1 && aPos.y == -1) // bottom right vertice
    {
        updated_position = end_position + perpendicular_angle * (thickness / 2);
    }
    // detect which vertice we are looking at, and move it to the correct position
    if (aPos.x == -1 && aPos.y == 1) // top left vertice
    {
        updated_position = start_position - perpendicular_angle * (thickness / 2);
    }
    if (aPos.x == -1 && aPos.y == -1) // bottom left vertice
    {
        updated_position = end_position - perpendicular_angle * (thickness / 2);
    }

    gl_Position = projection_matrix * vec4(updated_position, layer, 1.0);
    frag_position = vec2(aPos.x, aPos.y);
}
