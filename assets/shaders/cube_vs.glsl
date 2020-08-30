#version 430  
          
layout(location = 0) uniform mat4 PVM;

layout(location = 0) in vec3 pos_attrib;
out vec3 dir;

void main(void)
{
   //Compute clip-space vertex position
   gl_Position = PVM*vec4(pos_attrib, 1.0);     //w = 1 becase this is a point
   dir = pos_attrib;
}