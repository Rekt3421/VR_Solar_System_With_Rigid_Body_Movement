#version 430            

const int LINE_MODE = 0;
const int POINT_MODE = 1;
const int QUAD_MODE = 2; //Draw thick lines as a quad

layout(location = 0) uniform mat4 PVM;
layout(location = 1) uniform mat4 PV;
layout(location = 51) uniform int mode = LINE_MODE;
layout(location = 52) uniform vec3 pos = vec3(-1.0);
layout(location = 53) uniform float length = 30.0;

layout(location = 0) in vec3 pos_attrib;
layout(location = 1) in vec2 tex_coord_attrib;
layout(location = 2) in vec3 normal_attrib;

vec4 line_verts[] = vec4[2](vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, -1.0, 1.0));
float ends[] = float[2](0.5, 1.0);

out float end;

void main(void)
{
   if(mode == LINE_MODE)
   {
      vec4 p = vec4(line_verts[gl_VertexID].xyz*length, 1.0);
      end = ends[gl_VertexID];
      gl_Position = PVM*p;
   }
   if(mode == QUAD_MODE)
   {
      vec4 clip[2];
      clip[0] = PVM*vec4(line_verts[0].xyz, 1.0);
      clip[1] = PVM*vec4(line_verts[1].xyz*length, 1.0);  
      
      vec2 ndc[2];
      ndc[0] = clip[0].xy/clip[0].w;
      ndc[1] = clip[1].xy/clip[1].w;  

      vec2 ndc_dir = normalize(ndc[1]-ndc[0]);
      vec2 n = vec2(-ndc_dir.y, ndc_dir.x);

      float dir = 0.0015*float(2*(gl_VertexID%2)-1);
      vec4 offset = vec4(n*dir, 0.0, 0.0);
      gl_Position = clip[gl_VertexID/2]+offset;
      end = ends[gl_VertexID/2];
   }
   if(mode == POINT_MODE)
   {
      gl_Position = PV*vec4(pos, 1.0);
      gl_Position.z -= 0.002; //fake polygon offset
      gl_PointSize = 15.0;
   }
}

