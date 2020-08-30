#version 400 
uniform mat4 PVM;           

in vec3 pos_attrib;

void main(void)
{
	 gl_Position = P*VM*vec4(pos_attrib, 1.0);
}


/*
To use this shadow map in a subsequent rendering pass:

layout(location = 6) uniform mat4 S;
...
out vec4 shadow_coord;
...
shadow_coord = Shadow*VM*vec4(pos_attrib, 1.0);

*/