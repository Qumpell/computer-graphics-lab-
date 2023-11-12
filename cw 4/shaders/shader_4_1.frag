#version 430 core
uniform vec3 color;
out vec4 out_color;

float linearizeDepth(float depth, float near, float far) {
    float z = depth * 2.0 - 1.0;  // Znormalizuj zakres [0, 1] do [-1, 1]
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	//out_color = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1);
	 // Zakres odleg�o�ci kamery od fragmentu (liniowe)
    float linearDistance = linearizeDepth(gl_FragCoord.z, 0.1, 1.0);
	  // Kolor zale�ny od odleg�o�ci
    //vec3 color = vec3(1.0, 0.0, 0.0) * linearDistance;
    //out_color = vec4(color, 1.0);
    
    
	//out_color = vec4(color.x,color.y,color.z,1);


    
    // Kolor t�a
    vec3 backgroundColor = vec3(0.0f, 0.3f, 0.3f);

    // Wsp�czynnik mg�y (mo�esz dostosowa� warto�� dla po��danego efektu)
    float fogFactor = linearDistance;

    // Zastosuj efekt mg�y mieszaj�c kolory obiektu i t�a
    vec3 finalColor = mix(color, backgroundColor, fogFactor);

    out_color = vec4(finalColor, 1.0);

    out_color = vec4(color.x,color.y,color.z,1);
}
