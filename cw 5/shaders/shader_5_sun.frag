#version 430 core

//uniform vec3 objectColor;
uniform vec3 color;
uniform vec3 cameraPos;
uniform float exposure;
in vec3 interpNormal;
in vec3 worldPos;
out vec4 out_color;
void main()
{
	//gl_FragColor = vec4(objectColor, 1.0);
	//gl_FragColor = vec4(color, 1.0);
	//out_color = vec4(color, 1.0);

	 // Normalizuj wektor normalny
    vec3 normal = normalize(interpNormal);

    // Kierunek patrzenia (viewer direction)
    //vec3 viewDir = normalize(-vec3(gl_FragCoord.xy, 1.0));
    vec3 viewDir = normalize(cameraPos - worldPos.xyz);
  
     float rim = max(0.0, dot(normal, viewDir));

    // Dodaj efekt zacienienia na brzegach, mo�esz dostosowa� parametry wed�ug potrzeb
//    float edgeIntensity = 1.5;  // Intensywno�� zacienienia
//    float edgeThreshold = 0.2;  // Minimalny k�t, przy kt�rym zaczyna dzia�a� efekt
//
//    // Zastosuj efekt zacienienia tylko dla pikseli spe�niaj�cych warunek k�ta
//    vec3 finalColor;
//    if (rim < edgeThreshold) {
//        rim = 1.0 - (1.0 - rim) * edgeIntensity;
//       finalColor= color * rim;
//    } else {
//        finalColor =  color;
//    }
//    finalColor = vec3(1.0) - exp(-finalColor * exposure);
//
//	out_color = vec4(finalColor, 1.0);
//    //out_color = vec4(color * rim, 1.0);


    // Kierunek �wiat�a (za��my, �e jest to wektor skierowany od punktu na powierzchni w stron� �r�d�a �wiat�a)
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // Znormalizowany wektor �wiat�a, dostosuj go do rzeczywistej pozycji �r�d�a �wiat�a

    // K�t pomi�dzy wektorem normalnym a kierunkiem �wiat�a
    float cosTheta = dot(normal, viewDir);

    // Zdefiniuj kolory dla kraw�dzi (czerwony) i �rodka (��ty)
    vec3 edgeColor = vec3(1.0, 0.0, 0.0);  // Czerwony
    vec3 centerColor = vec3(1.0, 1.0, 0.0);  // ��ty

    // Modyfikuj kolor w zale�no�ci od k�ta
    vec3 finalColor = mix(edgeColor, centerColor, cosTheta);

    out_color = vec4(finalColor, 1.0);
 }