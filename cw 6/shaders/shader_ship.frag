#version 430 core

float AMBIENT = 0.1;

//uniform vec3 color;
uniform vec3 lightPos;
uniform sampler2D colorTexture;
uniform sampler2D scratchesTexture;
uniform sampler2D rustTexture;

in vec3 vecNormal;
in vec3 worldPos;
in vec2 vecTexCoord;

out vec4 outColor;
void main()
{
	vec3 lightDir = normalize(lightPos-worldPos);
	vec3 normal = normalize(vecNormal);
	float diffuse=max(0,dot(normal,lightDir));
	//outColor = vec4(color*min(1,AMBIENT+diffuse), 1.0);
		

    vec4 texturecolor = texture2D(colorTexture, vecTexCoord);
	vec4 scratchesColor = texture2D(scratchesTexture, vecTexCoord);
	vec4 rustColor = texture2D(rustTexture, vecTexCoord);
	vec4 finalColor = mix(texturecolor,rustColor, scratchesColor.r);
    outColor = vec4(finalColor.rgb, 1.0);
	//outColor = finalColor;

}