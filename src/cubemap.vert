#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 localPos;

uniform mat4 captProjection;
uniform mat4 captView;

void main()
{
    localPos = aPos;
    gl_Position =  captProjection * captView * vec4(localPos, 1.0);
}
