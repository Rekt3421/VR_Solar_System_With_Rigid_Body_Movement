#version 430            
layout(location = 0) uniform mat4 PVM;

layout(location = 51) uniform mat4 G = mat4(0.0, -10.0, 0.0, 0.0,  //acc
											0.0, 10.0, -10.0, 0.0,  //vel
											0.0, 0.0, 0.0, 1.0,     //pos
											0.0, 0.0, 0.0, 0.0);

//USING vec4 for pos attributes
layout(location = 0) in vec4 pos_attrib; //=(t*t, t, 1.0, 0.0)



void main(void)
{
	vec4 p = G*pos_attrib;
	gl_Position = PVM*p;
}

