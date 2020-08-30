#version 430

out vec4 fragcolor;   

in VertexData{
    vec2 mTexCoord;
} VertexIn;
       
      
void main(void)
{   
   //if(VertexIn.mTexCoord.x > 0.5) discard; //for dotted line

	fragcolor = vec4(0.5, 0.5, 1.0, 1.0);
}




















