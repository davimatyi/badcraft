#version 330 core

in vec2 vs_in_pos;

void main()
{
	gl_Position = vec4(vs_in_pos, 0, 1);
}