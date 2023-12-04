#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;
uniform float hp;

// Output
out vec3 color;
out float hp_out;

void main()
{
    color = object_color;
    hp_out = hp;
    vec3 position = vec3(v_position.x + (hp / 100 - 1) * sin(v_position.x) / 2, v_position.y, v_position.z - (hp / 100 - 1) * cos(v_position.z) / 2);
    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
