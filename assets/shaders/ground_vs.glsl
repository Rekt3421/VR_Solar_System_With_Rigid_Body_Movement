#version 430            
layout(location = 0) uniform mat4 PVM;
layout(location = 6) uniform mat4 S;

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;

out vec2 tex_coord;  
out vec4 shadow_coord;

void main(void)
{
   gl_Position = PVM*vec4(pos_attrib, 1.0);
   tex_coord = 0.5*(pos_attrib.xy + vec2(1.0));
   shadow_coord = S*vec4(pos_attrib, 1.0);
}