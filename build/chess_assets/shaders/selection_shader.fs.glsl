#version 330 core

out vec4 FragColor;

in vec3 fragment_position;
in vec2 tex_coords;

uniform int code;
uniform bool texture_enabled;

void main()
{
	if (texture_enabled)
	{
		int x_row = int((tex_coords.x - 0.703) / 0.036);
		int y_row = int((tex_coords.y - 0.709) / 0.0353);
		
		int cell = 8 * y_row + x_row + 100;
	
		FragColor = vec4(cell / 255.0, 0.0, 0.0, 0.0);
	}
	
	else
		FragColor = vec4(code / 255.0, 0.0, 0.0, 0.0);
}
