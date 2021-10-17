#version 330 core

out vec4 FragColor;

#define NUMBER_OF_TEXTURES 32

in vec3 fragment_position;
in vec2 tex_coords;

uniform int number_of_diffuse_textures;
uniform int number_of_specular_textures;
uniform int number_of_normal_maps;
uniform int number_of_height_maps;

uniform sampler2D texture_diffuse[NUMBER_OF_TEXTURES];
uniform sampler2D texture_specular[NUMBER_OF_TEXTURES];
uniform sampler2D texture_normal[NUMBER_OF_TEXTURES];
uniform sampler2D texture_height[NUMBER_OF_TEXTURES];

void main()
{
	for (int i = 0; i < number_of_diffuse_textures; ++i)
		FragColor = texture(texture_diffuse[i], tex_coords);
}
