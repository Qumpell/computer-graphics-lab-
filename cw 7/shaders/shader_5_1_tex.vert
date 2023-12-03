#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

in vec3 cameraPosition;
in vec3 lightPosition;

uniform mat4 transformation;
uniform mat4 modelMatrix;

out vec3 vecNormal;
out vec3 worldPos;
out vec2 vecTex;
out vec3 viewDirTS;
out vec3 lightDirTS;

void main()
{
	worldPos = (modelMatrix* vec4(vertexPosition,1)).xyz;
	vecNormal = (modelMatrix* vec4(vertexNormal,0)).xyz;
	vecTex = vertexTexCoord;
	vecTex.y = 1.0 - vecTex.y;
	gl_Position = transformation * vec4(vertexPosition, 1.0);
	vec3 tangent = (modelMatrix* vec4(vertexTangent,0)).xyz;
	vec3 bitangent = (modelMatrix* vec4(vertexBitangent,0)).xyz;
	vec3 normal = vecNormal;

	mat3 TBN = transpose(mat3(tangent, bitangent, normal));
	vec3 viewDir = normalize(cameraPosition - worldPos);
	vec3 lightDir = normalize(lightPosition - worldPos);
	viewDirTS = TBN * viewDir;
	lightDirTS = TBN * lightDir;


}
