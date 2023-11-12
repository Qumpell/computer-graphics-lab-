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
	 // Zakres odleg³oœci kamery od fragmentu (liniowe)
    float linearDistance = linearizeDepth(gl_FragCoord.z, 0.1, 1.0);
	  // Kolor zale¿ny od odleg³oœci
    //vec3 color = vec3(1.0, 0.0, 0.0) * linearDistance;
    //out_color = vec4(color, 1.0);
    
    
	//out_color = vec4(color.x,color.y,color.z,1);


    
    // Kolor t³a
    vec3 backgroundColor = vec3(0.0f, 0.3f, 0.3f);

    // Wspó³czynnik mg³y (mo¿esz dostosowaæ wartoœæ dla po¿¹danego efektu)
    float fogFactor = linearDistance;

    // Zastosuj efekt mg³y mieszaj¹c kolory obiektu i t³a
    vec3 finalColor = mix(color, backgroundColor, fogFactor);

    out_color = vec4(finalColor, 1.0);

    out_color = vec4(color.x,color.y,color.z,1);
}
