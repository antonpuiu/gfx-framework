#version 330

// Input
in vec3 color;
in float hp_out;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 modifier = (100 - hp_out) * vec3(1, 0, 0) / 100.0;
    out_color = vec4(color + modifier, 1.0);
}
