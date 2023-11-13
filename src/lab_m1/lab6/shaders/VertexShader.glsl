#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float time;

// Output
// TODO(student): Output values to fragment shader
out vec3 color;

void main()
{
    // TODO(student): Send output to fragment shader
    color = v_normal + vec3(sin(time), cos(time), 0.0);

    // TODO(student): change the position and color based on a time function.
    vec3 position = v_position + vec3(2.0 * sin(time), 1.5 * cos(time), 0);

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
