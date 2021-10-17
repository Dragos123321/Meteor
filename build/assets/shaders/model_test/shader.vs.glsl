#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

#define NUMBER_OF_MODEL_MATRICES 80

out vec3 fragment_position;
out vec2 tex_coords;

uniform mat4 model[NUMBER_OF_MODEL_MATRICES];
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragment_position = vec3(model[gl_InstanceID] * vec4(aPos, 1.0));
    tex_coords = aTexCoords;

    gl_Position = projection * view * model[gl_InstanceID] * vec4(aPos, 1.0);
}