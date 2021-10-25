#version 450 core

layout(location = 0) in vec2 vs_in_pos;
layout(location = 1) in vec2 vs_in_tex;
layout(location = 2) in vec4 vs_in_col;
layout(location = 3) in uint vs_in_slot;

layout(location = 0) out vec2 vs_out_pos;
layout(location = 1) out vec2 vs_out_tex;
layout(location = 2) out vec4 vs_out_col;
layout(location = 3) out flat uint vs_out_slot;

uniform mat4 viewTrafo;


void main()
{
	vs_out_pos = vs_in_pos;
	vs_out_tex = vs_in_tex;
	vs_out_col = vs_in_col;
	vs_out_slot = vs_in_slot;

	gl_Position = viewTrafo * vec4(vs_in_pos, 1.0, 1.0);
}