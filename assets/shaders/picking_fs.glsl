
#version 450
layout(location = 0) uniform mat4 PVM;

layout(location = 10) uniform int Pass = 1;

layout(location = 40) uniform int Id = -1;
layout(location = 41) uniform int PickedId[] = int[2](-1, -1);
layout(location = 43) uniform vec2 Mouseover_coord[] = vec2[2](vec2(-1.0), vec2(-1.0));

layout(location = 0) out vec4 fragcolor;
layout(location = 1) out vec4 pick;

in vec3 normal;
in vec2 tex_coord;

void main(void)
{   
   fragcolor = vec4(1.0, 1.0, 1.0, 1.0);
   pick = vec4(uintBitsToFloat(Id), tex_coord.x, tex_coord.y, 1.0);
}

