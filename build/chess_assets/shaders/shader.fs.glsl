#version 330 core

out vec4 FragColor;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NUMBER_OF_POINT_LIGHTS 2

in vec3 fragment_position;
in vec3 normal;
in vec2 tex_coords;

uniform vec3 diffuse_color;
uniform PointLight point_light[NUMBER_OF_POINT_LIGHTS];
uniform vec3 view_position;

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_direction);

void main()
{
	vec3 norm = normalize(normal);
    vec3 view_direction = normalize(view_position - fragment_position);
	
	vec3 result = vec3(0.0);
	
	for(int i = 0; i < NUMBER_OF_POINT_LIGHTS; ++i)
        result += calc_point_light(point_light[i], norm, fragment_position, view_direction);  
	
	FragColor = vec4(result, 1.0);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_direction)
{
	vec3 light_direction = normalize(light.position - frag_pos);

    float diff = max(dot(normal, light_direction), 0.0);

    vec3 halfway_direction = normalize(light_direction + view_direction);
    float spec = pow(max(dot(normal, halfway_direction), 0.0), 32.0);

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
			       light.quadratic * distance * distance);

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	ambient = light.ambient * diffuse_color;
	diffuse = light.diffuse * diff * diffuse_color;
	
	specular = light.specular * spec * diffuse_color;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}