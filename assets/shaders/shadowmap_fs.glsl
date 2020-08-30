#version 400

out vec4 fragcolor;           
  
void main(void)
{   
	fragcolor = vec4(1.0-gl_FragCoord.z*gl_FragCoord.w); //this is just a color for us to visualize the shadow map 	
}

/*
   To use this shadow map in a subsequent rendering pass:

   layout(location = 31) uniform sampler2DShadow Shadow_map;
...
   in vec4 shadow_coord;
...
   float lit = textureProj(Shadow_map, shadow_coord);

*/

