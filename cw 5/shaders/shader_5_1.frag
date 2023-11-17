#version 430 core

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform float exposure;

in vec3 vertNormal;
in vec3 vertPosition;


out vec4 outColor;
void main()
{	

	// Znormalizuj wektor normalny
	vec3 normal = normalize(vertNormal);

	// Oblicz kierunek �wiat�a punktowego
    vec3 lightDir = normalize(lightPos - vertPosition.xyz);

	//Oblicz iloczyn skalarny wektora normalnego iodwrotnego wektora kierunku �wiat�a
    float diffuseIntensity = max(dot(normal, lightDir), 0.0);


	// Oblicz wektor kierunku odbicia �wiat�a (specular)
    vec3 viewDir = normalize(cameraPos - vertPosition.xyz);
    vec3 reflectDir = reflect(-lightDir, normal);
	
	 // Oblicz nat�enie �wiat�a odbitego (specular)
    float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), 50.0);  // Dostosuj pot�g� do efektu po�ysku

    // Obliczenia os�abienia �wiat�a (attenuation)
    float distance = length(vertPosition - lightPos);
    vec3 attentuation = lightColor / (distance * distance);


	//vec3 finalColor = color * diffuseIntensity + lightColor * specularIntensity;

	vec3 finalColor = color * diffuseIntensity + attentuation * specularIntensity;
	float exposure1 = max(exposure, 0.1);

    // Ekspozycja
    finalColor = vec3(1.0) - exp(-finalColor * exposure1);


	outColor = vec4(finalColor, 1.0);
}
