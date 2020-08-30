#pragma once

namespace UniformLoc
{
   //Transformation matrices
   const int PVM = 0;             //mat4
   const int PV = 1;              //mat4
   const int VM = 2;              //mat4
   const int P = 3;               //mat4
   const int V = 4;               //mat4
   const int M = 5;               //mat4
   const int S = 6;               //mat4

   const int Pass = 10;            //int

   //Colors
   const int Diffuse_color = 20;   //vec4

   //Textures
   const int Diffuse_tex = 30;      //sampler2D
   const int Sky_cube = 31;         //samplerCube
   const int Shadow_map = 32;       //sampler2DShadow
   const int Paint_tex = 33;        //sampler2D

   //Picking
   const int Id = 40;              //int
   const int PickedId = 41;        //int[2]
   const int MouseoverId = 43;     //int[2]
   const int Mouseover_coord = 45; //vec3[2]

   //50s are object specific uniforms


};

namespace AttribLoc
{
   const int pos = 0;         //vec3
   const int tex_coord = 1;   //vec2
   const int normal = 2;      //vec3
   
};