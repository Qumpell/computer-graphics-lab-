#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 perspectiveMatrix;
void main()
{
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}