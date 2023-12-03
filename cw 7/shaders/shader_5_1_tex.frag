#version 430 core

float AMBIENT = 0.1;

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform sampler2D colorTexture;
uniform sampler2D normalSampler;
//in vec3 vecNormal;
//in vec3 worldPos;
in vec2 vecTex;
in vec3 viewDirTS;
in vec3 lightDirTS;

out vec4 outColor;
out vec3 lightPosition;
out vec3 cameraPosition;
void main()
{
	vec3 lightDir = normalize(lightDirTS);
//	vec3 normal = normalize(vecNormal);
	vec3 V = normalize(viewDirTS);
	vec4 N = texture2D(normalSampler, vecTex);
	N = normalize(N *2.0 - 1.0);
	
	float diffuse=max(0,dot(N.xyz,lightDir));
	vec4 textureColor = texture2D(colorTexture, vecTex);

    vec4 finalColor = mix(textureColor, vec4(1.0) ,N.r);
//	outColor = vec4(textureColor*min(1,AMBIENT+diffuse), 1.0);
	outColor = vec4(finalColor.rgb*min(1,AMBIENT+diffuse), 1.0);
	//outColor = finalColor;
	lightPosition = lightPos;
	cameraPosition = cameraPos;
}
