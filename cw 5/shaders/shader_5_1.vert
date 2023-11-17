#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 transformation;
uniform mat4 modelMatrix;

out vec3 vertNormal;
out vec3 vertPosition;
void main()
{
	gl_Position = transformation * vec4(vertexPosition, 1.0);
	vertNormal = (modelMatrix* vec4(vertexNormal,0)).xyz;
	vertPosition = (modelMatrix* vec4(vertexPosition,1)).xyz;
}
