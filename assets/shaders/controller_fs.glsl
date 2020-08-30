#version 430 core

layout(location = 1) uniform sampler2D diffuse;

in vec2 v2TexCoord;
out vec4 outputColor;
void main()
{
   outputColor = texture(diffuse, v2TexCoord);
}
