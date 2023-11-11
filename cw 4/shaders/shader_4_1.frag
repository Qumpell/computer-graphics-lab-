#version 430 core
uniform vec3 color;
out vec4 out_color;


void main()
{
	//out_color = vec4(1,0,1,1);
	out_color = vec4(color.x,color.y,color.z,1);
}
