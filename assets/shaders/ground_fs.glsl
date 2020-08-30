#version 430

layout(location = 30) uniform sampler2D Diffuse_tex;
layout(location = 32) uniform sampler2DShadow Shadow_map;

layout(location = 40) uniform int Id = -1;


out vec4 fragcolor;           
in vec2 tex_coord;
in vec4 shadow_coord;
    
void main(void)
{   
	fragcolor = texture(Diffuse_tex, 100.0*tex_coord);
   float lit = textureProj(Shadow_map, shadow_coord);
   lit = (lit+0.5)/1.5;   

   fragcolor.rgb *= lit;

   float r = length(tex_coord-vec2(0.5));
   fragcolor.a = smoothstep(0.5, 0.4, r);
}




















