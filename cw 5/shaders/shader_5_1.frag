#version 430 core

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform float exposure;
uniform vec3 spotPos;
uniform vec3 spotDir;

in vec3 vertNormal;
in vec3 vertPosition;


out vec4 outColor;



void main()
{	
    float reflectorAngle = 60.0f;
    
	// Znormalizuj wektor normalny
	vec3 normal = normalize(vertNormal);

	// Oblicz kierunek œwiat³a punktowego
    vec3 lightDir = normalize(lightPos - vertPosition.xyz);
    vec3 lightDir2 = normalize(spotPos - vertPosition.xyz);
   //vec3 lightDir2 = normalize(spotDir);


    vec3 lightining2;
    bool check = false;
    float angle1 = dot(lightDir2, normalize(-spotDir));
	if(angle1 < cos(radians(reflectorAngle))){
        check = true;
    }
    
    
    //Oblicz iloczyn skalarny wektora normalnego iodwrotnego wektora kierunku œwiat³a
    float diffuseIntensity = max(dot(normal, lightDir), 0.0);
    float diffuseIntensity2 = max(dot(normal, lightDir2), 0.0);



	// Oblicz wektor kierunku odbicia œwiat³a (specular)
    vec3 viewDir = normalize(cameraPos - vertPosition.xyz);

    vec3 reflectDir = reflect(-lightDir, normal);
	vec3 reflectDir2 = reflect(-lightDir2, normal);

	 // Oblicz natê¿enie œwiat³a odbitego (specular)
    float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), 50.0);  // Dostosuj potêgê do efektu po³ysku

    float specularIntensity2 = pow(max(dot(viewDir, reflectDir2), 0.0), 50.0);

    // Obliczenia os³abienia œwiat³a (attenuation)
    float distance = length(vertPosition - lightPos);
     float distance2 = length(vertPosition - spotPos);
    vec3 attentuation = lightColor / (distance * distance);
    vec3 attentuation2 = lightColor / (distance2 * distance2);

	//vec3 finalColor = color * diffuseIntensity + lightColor * specularIntensity;

	vec3 lighting1 = diffuseIntensity + attentuation * specularIntensity;
    vec3 lighting2 = vec3(0.0f,0.0f,0.0f);
    if(!check){
        lighting2 = diffuseIntensity2 + attentuation2 * specularIntensity2;
    }
    
    //lighting2 = diffuseIntensity2 + attentuation2 * specularIntensity2;

    


      // Sumuj oœwietlenie z wszystkich Ÿróde³
    vec3 finalColor = color * (lighting1 + lighting2);

    
    // Ekspozycja
    float exposure1 = max(exposure, 0.1);
    finalColor = vec3(1.0) - exp(-finalColor * exposure1);

	outColor = vec4(finalColor, 1.0);
}
