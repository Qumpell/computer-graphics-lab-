#version 430 core

//uniform vec3 objectColor;
uniform vec3 color;

in vec3 interpNormal;

void main()
{
	//gl_FragColor = vec4(objectColor, 1.0);
	gl_FragColor = vec4(color, 1.0);
}
