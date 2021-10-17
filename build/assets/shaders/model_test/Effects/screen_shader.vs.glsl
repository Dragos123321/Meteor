#version 430 core

layout(location = 0) in vec2 vert_position;
layout(location = 1) in vec2 vert_tex_coords;
layout(location = 2) in vec4 vert_color;

out vec2 frag_tex_coords;
out vec4 frag_color;

uniform mat4 camera_matrix;

void main()
{
	gl_Position = vec4((camera_matrix * vec4(vert_position, 0.0, 1.0)).xy, 0.0, 1.0);
	frag_tex_coords = vert_tex_coords;
	frag_color = vert_color;
}