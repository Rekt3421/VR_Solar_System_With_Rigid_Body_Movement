#version 430

layout(location = 30) uniform sampler2D Diffuse_tex;
layout(location = 32) uniform sampler2DShadow Shadow_map;

layout(location = 40) uniform int Id = -1;
layout(location = 41) uniform int PickedId[] = int[2](-1, -1);
layout(location = 43) uniform int MouseoverId[] = int[2](-1, -1);
layout(location = 45) uniform vec3 Mouseover_coord[] = vec3[2](vec3(-1.0), vec3(-1.0));

out vec4 fragcolor;           
in vec2 tex_coord;
in vec4 shadow_coord;

const vec3 hand_colors[2] = vec3[2](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
      
void main(void)
{   
	fragcolor = texture(Diffuse_tex, tex_coord);
   float lit = textureProj(Shadow_map, shadow_coord);
   lit = (lit+0.5)/1.5;   

   fragcolor.rgb *= lit;

   for(int i=0; i<2; i++)
   {
      if (Id == PickedId[i])
      {
         fragcolor.rgb *= 0.5*hand_colors[i]+vec3(1.0);
      }

      if (Id == MouseoverId[i])
      {
         fragcolor.rgb *= 0.5*(hand_colors[i]+vec3(1.0));
      }
   }

   

   float spot[2] = float[2](0.0,0.0);
   vec4 spotcolor = vec4(0.0, 0.0, 0.0, 1.0);
   for(int i=0;i<2; i++)
   {
      if (Mouseover_coord[i].x > 0.0 && Mouseover_coord[i].y > 0.0 && Id == MouseoverId[i])
      {
         float d = distance(tex_coord, Mouseover_coord[i].xy);
         spot[i] = smoothstep(0.1, 0.09, d) - smoothstep(0.05, 0.04, d);
         spotcolor.rgb += spot[i]*hand_colors[i];
      }
   }
   float s = max(spot[0], spot[1]);

   fragcolor = mix(fragcolor, spotcolor, s);

   //fragcolor = vec4(tex_coord, 0.0, 1.0);
}




















