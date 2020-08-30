#pragma once
#include <windows.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>

class RenderSceneNode;

namespace VrScene
{
   void initScene(GLFWwindow* win, int w, int h);
   void updateScene();
   void drawScene(const glm::mat4& P, const glm::mat4& V);
   void drawScenePass(const glm::mat4& P, const glm::mat4& V, int pass);
   void drawGui();
   void draw3DGuiTextures();
   void keyboardScene();
   void mouseScene();

   void lockShader();
   void unlockShader();
   glm::mat4 getBodyToWorldMatrix();
   void setHeadToWorldMatrix(const glm::mat4& HtoW);
   glm::mat4 getHeadToWorldMatrix();

   RenderSceneNode* GetRootNode();

   enum RenderPass
   {
      PickPass,
      ShadowPass,
      DisplayPass,
   };

   enum SelectMode //interaction mode?
   {
      NONE,
      FREEHAND,
      WIDGET,
      TOUCHPAD,
      PAINT
   };

   extern SelectMode selectMode[2]; //initially = FREEHAND;
};


