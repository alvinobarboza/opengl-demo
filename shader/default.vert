#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 current_pos;

uniform float scale;

uniform mat4 camera_matrix;
uniform mat4 model;

void main()
{
    current_pos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = camera_matrix * vec4(current_pos, 1.0);
    color = aColor;
    texCoord = aTex;
    normal = aNormal;
}