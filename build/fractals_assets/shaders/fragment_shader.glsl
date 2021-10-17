#version 330 core

out vec4 FragColor;

uniform float rect_width;
uniform float rect_height;
uniform vec2 area_w;
uniform vec2 area_h;
uniform int max_iterations;

uniform vec3 color_map[17];

void main()
{
vec2 C = vec2(gl_FragCoord.x * (area_w.y - area_w.x) / rect_width  + area_w.x,
				  gl_FragCoord.y * (area_h.y - area_h.x) / rect_height + area_h.x);
	
	vec2 Z = vec2(0.0);
	
	int iteration = 0;
	
	while (iteration < max_iterations)
	{
		float x = Z.x * Z.x - Z.y * Z.y + C.x;
		float y = 2.0 * Z.x * Z.y       + C.y;
		
		if (x * x + y * y > 4.0)
			break;
			
		Z.x = x;
		Z.y = y;
		
		++iteration;
	}
	
	int row_index = (iteration * 100 / max_iterations % 17);
	FragColor = vec4((iteration == max_iterations ? vec3(0.0) : color_map[row_index]), 1.0);
}
