#version 430

layout(location=31) uniform samplerCube Sky_cube;

out vec4 fragcolor;           
in vec3 dir;
     
void main(void)
{   
   //fragcolor = vec4(pos, 1.0);
   fragcolor = mix(1.5*vec4(0.85, 0.5, 0.4, 1.0), 1.5*vec4(0.4, 0.5, 0.9, 1.0), smoothstep(-0.3, 0.2, dir.y));
}




















