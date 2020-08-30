#version 430

layout(location = 50) uniform int hand;

out vec4 fragcolor;   

in VertexData{
    vec2 mTexCoord;
} VertexIn;
       

const vec3 hand_colors[2] = vec3[2](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
      
void main(void)
{   
   if(VertexIn.mTexCoord.x > 0.5) discard;

   vec3 color = hand_colors[hand];
	fragcolor = vec4(color, 1.0);
}




















