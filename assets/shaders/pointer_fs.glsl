#version 430

const int LINE_MODE = 0;
const int POINT_MODE = 1;
const int QUAD_MODE = 2; //Draw thick lines as a quad

layout(location = 50) uniform int hand;
layout(location = 51) uniform int mode = LINE_MODE;

out vec4 fragcolor;           

in float end;

const vec3 hand_colors[2] = vec3[2](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
      
void main(void)
{   
   if(mode == LINE_MODE || mode == QUAD_MODE)
   {
      vec3 color = end*hand_colors[hand];
	   fragcolor = vec4(color, 0.5);
      return;
   }
   if(mode == POINT_MODE)
   {
      vec3 color = hand_colors[hand];
      vec2 p = 2.0*gl_PointCoord-vec2(1.0);
      if(dot(p,p) >= 1.0) discard;
      float a = 1.0-smoothstep(0.9, 0.999, dot(p,p));
	   fragcolor = vec4(color*a, a);
      return;
   }
   fragcolor = vec4(1.0);
}




















