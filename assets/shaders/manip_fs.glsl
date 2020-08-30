#version 430

//layout(location = 0) uniform mat4 PVM;
//layout(location = 2) uniform mat4 VM;

//layout(location = 10) uniform int Pass;

layout(location = 40) uniform int Id = -1;
layout(location = 45) uniform vec3 Mouseover_coord[] = vec3[2](vec3(-1.0), vec3(-1.0));

layout(location = 50) uniform int Transform = 0;
layout(location = 51) uniform vec3 Scale = vec3(1.0);
layout(location = 52) uniform float MouseOverAxis = -1.0;

layout(location = 0) out vec4 fragcolor;
layout(location = 1) out vec4 pick;  
         
in vec3 pos; 
noperspective in vec2 tex_coord; 
flat in vec4 color;
flat in float plane;
flat in float view_ang;

const int TranslateTransform = 0;
const int RotateTransform = 1;
const int ScaleTransform = 2;

const float cw = 0.05; //circle width
const float lw = 0.04; //line width
const vec2 arrowAngle = normalize(vec2(0.05, 0.02)); //arrowhead angle

float udSphere( vec3 p, vec3 cen, float r )
{
  p=p-cen;
  return abs(length(p)-r);
}

float sdSphere( vec3 p, vec3 cen, float r )
{
  p=p-cen;
  return length(p)-r;
}

float udAxes(vec2 p)
{
   vec2 abs_p = abs(p);
   return min(abs_p.x, abs_p.y);
}

float udBox( vec3 p, vec3 cen, vec3 b )
{
  p=p-cen;
  return length(max(abs(p)-b,0.0));
}

float sdBox( vec3 p, vec3 cen, vec3 b )
{
   p=p-cen;
   vec3 d = abs(p) - b;
   return min(max(d.z, max(d.x,d.y)),0.0) + length(max(d,0.0));
}

float sdCone( vec3 p, vec3 cen, vec2 c )
{
    p=p-cen;
    // c must be normalized
    float q = length(p.xy);
    return dot(c, vec2(q, p.z));
}

float opI( float d1, float d2 )
{
    return max(d1,d2);
}

float opS( float d1, float d2 )
{
    return max(-d1,d2);
}

float sdArrowhead(vec3 p, vec3 cen, vec2 c)
{
   float d1 = sdSphere(p, cen, 0.2);
   float d2 = sdCone(p, cen, c);
   return opI(d1,d2);
}

float dTxy(vec3 p)
{
   float d = sdBox(p.xyz, vec3(0.5, 0.5, 0.0), vec3(0.1));
   return d;
}

float dTxz(vec3 p)
{
   float d = sdBox(p.xyz, vec3(0.5, 0.0, 0.5), vec3(0.1));
   return d;
}

float dTyz(vec3 p)
{
   float d = sdBox(p.xyz, vec3(0.0, 0.5, 0.5), vec3(0.1));
   return d;
}

float dTx(vec3 p)
{
   float d1 = abs(length(p.yz))-lw;
   float ds = sdSphere(p, vec3(0.0), 0.80);
   d1 = opI(d1, ds);
   float d2 = sdArrowhead(p.zyx, vec3(0.0, 0.0, 0.95), arrowAngle);
   return min(d1,d2);
}

float dTy(vec3 p)
{

   float d1 = abs(length(p.xz))-lw;
   float ds = sdSphere(p, vec3(0.0), 0.80);
   d1 = opI(d1, ds);
   float d2 = sdArrowhead(p.zxy, vec3(0.0, 0.0, 0.95), arrowAngle);
   return min(d1,d2);
}

float dTz(vec3 p)
{

   float d1 = abs(length(p.xy))-lw;
   float ds = sdSphere(p, vec3(0.0), 0.80);
   d1 = opI(d1, ds);
   float d2 = sdArrowhead(p.xyz, vec3(0.0, 0.0, 0.95), arrowAngle);
   return min(d1,d2);
}


float dSx(vec3 p)
{
   float d1 = abs(length(p.yz))-lw;
   float ds = sdSphere(p, vec3(0.0), 0.90);
   d1 = opI(d1, ds);
   float d2 = sdBox(p.xyz, vec3(0.90, 0.0, 0.0), vec3(0.05)/Scale);
   return min(d1,d2);
}

float dSy(vec3 p)
{

   float d1 = abs(length(p.xz))-lw;
   float ds = sdSphere(p, vec3(0.0), 0.90);
   d1 = opI(d1, ds);
   float d2 = sdBox(p.xyz, vec3(0.0, 0.90, 0.0), vec3(0.05)/Scale);
   return min(d1,d2);
}

float dSz(vec3 p)
{

   float d1 = abs(length(p.xy))-lw;
   float ds = sdSphere(p, vec3(0.0), 0.90);
   d1 = opI(d1, ds);
   float d2 = sdBox(p.xyz, vec3(0.0, 0.0, 0.90), vec3(0.05)/Scale);
   return min(d1,d2);
}

float dRx(vec3 p)
{
   float d1 = abs(p.x);
   const float r = 0.9;
   float d2 = abs(length(pos.yz)-r)-cw; //circle
   return opI(d1,d2);
}

float dRy(vec3 p)
{
   float d1 = abs(p.y);
   const float r = 0.9;
   float d2 = abs(length(pos.xz)-r)-cw; //circle
   return opI(d1,d2);
}

float dRz(vec3 p)
{
   float d1 = abs(p.z);
   const float r = 0.9;
   float d2 = abs(length(pos.xy)-r)-cw; //circle
   return opI(d1,d2);
}

float dR2(vec3 p)
{
   float d2 = sdSphere(p, vec3(0.0), 0.4 );
   float d1 = sdSphere(p, vec3(0.0), 0.3 );
   return opS(d1, d2);
}




void main(void)
{   
   vec4 c = vec4(0.0);
   float dist = 100.0;
   float axis = -1.0;

      if(Transform == TranslateTransform)
      {
         float tx = dTx(pos);
         float ty = dTy(pos);
         float tz = dTz(pos);

         float txy = dTxy(pos);
         float txz = dTxz(pos);
         float tyz = dTyz(pos);

         if(tx<=dist)
         {
            dist = tx;
            c = vec4(1.0, 0.0, 0.0, 1.0);
            axis = 0.0;
         }
         if(ty<=dist)
         {
            dist = ty;
            c = vec4(0.0, 1.0, 0.0, 1.0);
            axis = 1.0;
         }
         if(tz<=dist)
         {
            dist = tz;
            c = vec4(0.0, 0.0, 1.0, 1.0);
            axis = 2.0;
         }

         if(txy<=dist)
         {
            dist = txy;
            c = vec4(0.0, 0.0, 1.0, 1.0);
            axis = 4.0;
         }

         if(txz<=dist)
         {
            dist = txz;
            c = vec4(0.0, 1.0, 0.0, 1.0);
            axis = 5.0;
         }

         if(tyz<=dist)
         {
            dist = tyz;
            c = vec4(1.0, 0.0, 0.0, 1.0);
            axis = 6.0;
         }

      }

      if(Transform == RotateTransform)
      {
         float rx = dRx(pos);
         float ry = dRy(pos);
         float rz = dRz(pos);
         if(rx<=dist)
         {
            dist = rx;
            c = vec4(1.0, 0.0, 0.0, 1.0); 
            axis = 0.0;
         }
         if(ry<=dist)
         {
            dist = ry;
            c = vec4(0.0, 1.0, 0.0, 1.0); 
            axis = 1.0;
         }
         if(rz<=dist)
         {
            dist = rz;
            c = vec4(0.0, 0.0, 1.0, 1.0); 
            axis = 2.0;
         }

         rx = dR2(pos);
         if(rx<=dist)
         {
            dist = rx;
            c = vec4(0.0, 1.0, 1.0, 1.0); 
            axis = 7.0;
         }

      }

      if(Transform == ScaleTransform)
      {
         float sx = dSx(pos);
         float sy = dSy(pos);
         float sz = dSz(pos);

         if(sx <= dist)
         {
            dist = sx;
            c = vec4(1.0, 0.0, 0.0, 1.0);
            axis = 0.0;
         }
         if(sy <= dist)
         {
            dist = sy;
            c = vec4(0.0, 1.0, 0.0, 1.0);
            axis = 1.0;
         }
         if(sz <= dist)
         {
            dist = sz;
            c = vec4(0.0, 0.0, 1.0, 1.0);
            axis = 2.0;
         }
      }

      float dbutton = length(pos*Scale)-0.2; //disk
      if(dbutton<dist)
      {
         dist = dbutton;
         c = vec4(1.0, 1.0, 1.0, 1.0); 
         axis = 3.0; //change transform
      }
      
   float fw = max(min(1.5*fwidth(dist), 0.01), 0.0001);

   const float w = 0.01;
   float a = 0.5f*view_ang*smoothstep(fw+w, w, dist);

   if (a <= 0.0)
   {
      discard;
   }

   if(axis == MouseOverAxis)
   {
      c = c+vec4(0.5);
      a = a*4.0;
   }

   fragcolor = vec4((1.0-smoothstep(w-fw, w, dist))*c.rgb, color.a*a);

   //pick.x == id
   //pick.y == plane
   //pick.z == axis
   pick = vec4(uintBitsToFloat(Id), plane, axis, 1.0);

}

