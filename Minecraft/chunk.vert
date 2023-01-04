#version 330 core

in ivec4 blockData;

out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

uniform mat4 MVP;

void main()
{
	int tex = blockData.w;
	int vertexID = tex & 0xff;
	int faceID = (tex >> 8) & 0xff;
	int textureID = (tex >> 16) - 1;

	vs_out_pos = blockData.xyz;

	gl_Position = MVP * vec4(blockData.xyz, 1);

	switch(faceID) 
	{
		case 0:
			vs_out_norm = vec3(1, 0, 0);
			break;
		case 1:
			vs_out_norm = vec3(-1, 0, 0);
			break;
		case 2:
			vs_out_norm = vec3(0, 1, 0);
			break;
		case 3:
			vs_out_norm = vec3(0, -1, 0);
			break;
		case 4:
			vs_out_norm = vec3(0, 0, 1);
			break;
		case 5:
			vs_out_norm = vec3(0, 0, -1);
			break;
	}

	vec2 texCoord = vec2((textureID % 16) / 16.f, 1 - ((textureID / 16) / 16.f + 1/16.f));
	switch(vertexID)
	{
		case 0: 
			vs_out_tex = texCoord;
			break;
		case 1:
			vs_out_tex = vec2(texCoord.x + 1/16.f, texCoord.y);
			break;
		case 2:
			vs_out_tex = vec2(texCoord.x, texCoord.y + 1/16.f);
			break;
		case 3:
			vs_out_tex = vec2(texCoord.x + 1/16.f, texCoord.y + 1/16.f);
			break;
	}
	


}