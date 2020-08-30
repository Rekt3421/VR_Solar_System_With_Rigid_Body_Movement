#version 430            
layout(location = 0) uniform mat4 PVM;
layout(location = 2) uniform mat4 VM;

layout(location = 10) uniform int Pass;

layout(location = 40) uniform int Id = -1;
layout(location = 45) uniform vec3 Mouseover_coord[] = vec3[2](vec3(-1.0), vec3(-1.0));

layout(location = 50) uniform int Transform = 0;
layout(location = 51) uniform vec3 Scale = vec3(1.0);

layout (location = 0) in vec3 pos_attrib;

out vec3 pos; 
noperspective out vec2 tex_coord; 
flat out vec4 color;
flat out float plane;
flat out float view_ang;

void main(void)
{
		gl_Position = PVM*vec4(pos_attrib, 1.0);
		pos = pos_attrib;

      tex_coord = vec2(0.0); 
      color = vec4(0.0);

      vec4 n;
      if(pos_attrib.x == 0.0)
      {
         tex_coord = pos_attrib.yz;
         color = vec4(1.0, 0.0, 0.0, 1.0);
         plane = 0.0;

         n = vec4(1.0, 0.0, 0.0, 0.0);

      }
      else if(pos_attrib.y == 0.0)
      {
         tex_coord = pos_attrib.xz;
         color = vec4(0.0, 1.0, 0.0, 1.0);
         plane = 1.0;

         n = vec4(0.0, 1.0, 0.0, 0.0);
      }
      else if(pos_attrib.z == 0.0)
      {
         tex_coord = pos_attrib.xy;
         color = vec4(0.0, 0.0, 1.0, 1.0);
         plane = 2.0;

         n = vec4(0.0, 0.0, 1.0, 0.0); 
      }


      vec4 nv = VM*n;
      vec4 ev = VM*vec4(0.0, 0.0, 0.0, 1.0);

      float s = min(1.0, abs(dot(nv,ev)));
      view_ang = s*s-0.05;
      //if(abs(dot(nv,ev)) < 0.1)
      {
         //gl_Position = PVM*vec4(s*pos_attrib, 1.0);
      }
}