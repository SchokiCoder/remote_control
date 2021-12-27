#version 330 core

layout(location = 0) in vec3 a_position;

out vec2 v_tex_coord;

void main()
{
    gl_Position = vec4(a_position, 1.0f);
    v_tex_coord = vec2(0.0f, 0.0f);
}
