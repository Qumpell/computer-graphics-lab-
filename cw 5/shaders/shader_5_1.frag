#version 430 core

uniform vec3 color;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 cameraPos;


in vec3 vecNormal;
in vec3 vecPosition;

out vec4 outColor;
void main()
{	
	
	vec3 normal = normalize(vecNormal);

	float diffuse=max(0,dot(normal,lightDir));

	vec3 viewDirectionVec = normalize(cameraPos-vecPosition);
	vec3 lightDirVecR =	reflect(-lightDir, vecNormal); 
	float specular =max(dot(viewDirectionVec,lightDirVecR), 0.0);
	specular = pow(specular, 50);
//	outColor = vec4(color, 1.0) * (lightColor * diffuse);
	outColor = vec4(color * diffuse * lightColor * specular, 1.0);
}
