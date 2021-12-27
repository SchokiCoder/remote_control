#version 120

attribute vec3 a_position;

varying vec2 v_texCoord;

void main()
{
    gl_Position = vec4(a_position, 1.0f);
    v_texCoord = vec2(0.0f, 1.0f);
}
