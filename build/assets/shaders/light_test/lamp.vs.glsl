#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aColor;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    color = aColor;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}