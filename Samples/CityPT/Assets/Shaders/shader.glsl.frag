#version 450 core

layout(location = 0) in vec2 fs_in_pos;
layout(location = 1) in vec2 fs_in_tex;
layout(location = 2) in vec4 fs_in_col;
layout(location = 3) in flat uint fs_in_slot;

layout(location = 0) out vec4 fs_out_col;

layout(binding = 0) uniform sampler2D textures[32];

void main()
{
	vec4 color = fs_in_col;
	if (fs_in_slot != 0) {
		color *= texture(textures[fs_in_slot - 1], fs_in_tex);
	}

	fs_out_col = color;
}