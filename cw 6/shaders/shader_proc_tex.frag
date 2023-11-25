#version 430 core

float AMBIENT = 0.1;

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 baseColor1;  // Base color when sin(localPos.y) > 0
uniform vec3 baseColor2;  // Base color when sin(localPos.y) < 0

in vec3 vecNormal;
in vec3 worldPos;

out vec4 outColor;
void main()
{
	vec3 lightDir = normalize(lightPos-worldPos);
	vec3 normal = normalize(vecNormal);
	float diffuse=max(0,dot(normal,lightDir));
	//outColor = vec4(color*min(1,AMBIENT+diffuse), 1.0);

    float sinValue = sin(worldPos.y);

    if (sinValue > 0.0) {
        outColor = vec4(baseColor1, 1.0);
    } else {
        outColor = vec4(baseColor2, 1.0);
    }


}
