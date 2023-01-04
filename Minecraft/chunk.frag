#version 330 core

in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform vec3 light_dir = vec3(0, -1, 0);

uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);

uniform sampler2D blockAtlas;

void main()
{
	vec3 ambient = La;

	vec3 normal = vs_out_norm;
	vec3 to_light = normalize(-light_dir);

	float cosa = clamp(dot(normal, to_light), 0, 1);
	vec3 diffuse = cosa * Ld;

	fs_out_col = vec4(ambient + diffuse, 1) * texture(blockAtlas, vs_out_tex);
}