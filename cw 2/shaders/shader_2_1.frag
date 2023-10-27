#version 430 core

flat in vec4 color;
in vec4 pos_local;
in vec4 pos_global;


uniform float time;
uniform vec4 backgroundColor;

out vec4 out_color;
void main()
{	
	//mieszanie sie tla z prostopadloscianem
	float fade = sin(time);
	//out_color = mix(color, backgroundColor, fade);
	
	//uzycie zmiennych do kolorow
	//out_color = pos_global;
	//out_color = pos_local;
	
	//zmiana koloru na czarny
	// out_color = color / time;

	//paski na prostopadloscianie
	float stripeGap = 1.0f;
	float stripeCount = 0.8f;
	float stripeWidth = 0.5f;
	float stripeCoordinate = abs(sin(time + pos_local.x * (stripeCount + stripeGap)));
	stripeCoordinate /= stripeWidth;



	//float stripeCoordinate = abs(sin(time + pos_local.x));
	 // Kolor paska
    vec3 stripeColor = vec3(stripeCoordinate, 0.3, 0.3);

	    // Ustawianie koloru dla wszystkich pasków
    for (int i = 0; i < int(stripeCount); ++i) {
        stripeColor = mix(stripeColor, vec3(0.3, 0.3, 0.3), smoothstep(float(i) / stripeCount, (float(i) + 1.0) / stripeCount, stripeCoordinate));
    }

    // Przypisanie koloru paska do wyjœciowej zmiennej FragColor
    out_color = vec4(stripeColor, 1.0) * color;
}
