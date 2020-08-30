#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VrScene.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "LoadShader.h"
#include "imgui.h"
#include "imgui_draw_3d.h"
#include "imgui_impl_vr.h"

#include "Audio.h"
#include "Picker.h"
#include "RenderNode.h"
#include "MeshNode.h"
#include "VrManipulator.h"
#include "VrScene3dGui.h"
#include "VrPointer.h"
#include "VrPointer.h"
#include "VrTeleporter.h"
#include "SkyNode.h"
#include "GroundNode.h"
#include "ShadowMapPass.h"
#include "LoadAssets.h"
#include "Component.h"

namespace VrScene
{
   GLFWwindow* window = nullptr;

   static const std::string ground_name = "tiles.jpg";

   static const std::string mesh_name = "Amago0.3ds";
   MeshData mesh_data;

   static const std::string texture_name = "AmagoT.bmp";
   GLuint texture_id = -1; //Texture map for fish

   static const std::string vertex_shader("template_vs.glsl");
   static const std::string fragment_shader("template_fs.glsl");


   bool shaderLock = false;

   glm::mat4 headToWorldMatrix(1.0f);

   Picker* gPicker = nullptr;
   VrManipulator* gManipulator[2] = { nullptr, nullptr }; 
   RenderSceneNode* gRootNode;

   ShadowMapPass* pShadowMapPass;
   Audio* gAudio;

   SelectMode selectMode[2] = {FREEHAND, FREEHAND};


   RenderSceneNode* GetRootNode() {return gRootNode;}
};

void VrScene::lockShader()
{
   shaderLock = true; //TODO remove
   RenderSceneNode::LockShader = true;
}

void VrScene::unlockShader()
{
   shaderLock = false; //TODO remove
   RenderSceneNode::LockShader = false;
}

glm::mat4 VrScene::getBodyToWorldMatrix()
{
   return RenderSceneNode::Mbody;    
}

void VrScene::setHeadToWorldMatrix(const glm::mat4& HtoW)
{
   headToWorldMatrix = HtoW;
}

glm::mat4 VrScene::getHeadToWorldMatrix()
{
   return headToWorldMatrix;
}

void VrScene::initScene(GLFWwindow* win, int w, int h)
{
   gAudio = new Audio();
   VrScene::window = win;

   Init3dGui();
   ImGui_Impl_VR_SetGuiScale(0.25f);
  
   const int pickw = 256;
   const int pickh = 256;
   gPicker = new Picker(pickw, pickh);

   const int shadow_map_size = 4096;
   pShadowMapPass = new ShadowMapPass(shadow_map_size);

   gRootNode = new RenderSceneNode;
   gRootNode->SetName("Scene Root");

   SkyNode* sky = CreateSky();
   gRootNode->AddChild(sky);

   GLuint ground_tex = LoadAssets::LoadTexture(ground_name.c_str());
   GroundNode* ground = CreateGround(ground_tex);
   gRootNode->AddChild(ground);

   {
      RotatorComponent* rot = new RotatorComponent;

      mesh_data = LoadAssets::LoadMesh(mesh_name);
      texture_id = LoadAssets::LoadTexture(texture_name.c_str());
      GLuint shader_program = LoadAssets::LoadShader(vertex_shader.c_str(), fragment_shader.c_str());
      TransformComponent* scale = new TransformComponent;
      scale->SetPreM(glm::scale(2.0f*glm::vec3(mesh_data.mScaleFactor)));

      TransformComponent* trans = new TransformComponent;
      trans->SetPreM(glm::translate(glm::vec3(0.2f, 0.2, 0.2)));

      MeshNode* pMeshNode = CreateMeshNode(shader_program, mesh_data, texture_id);
      //pMeshNode->AddTransformComponent(scale);
      gRootNode->AddChild(pMeshNode);

      MeshNode* pMeshNode2 = CreateMeshNode(shader_program, mesh_data, texture_id);
      //pMeshNode2->AddTickComponent(rot);
      //pMeshNode2->AddTransformComponent(rot);
      pMeshNode2->AddTransformComponent(trans);
      pMeshNode->AddChild(pMeshNode2);

      MeshNode* pMeshNode3 = CreateMeshNode(shader_program, mesh_data, texture_id);
      pMeshNode3->AddTransformComponent(trans);
      pMeshNode2->AddChild(pMeshNode3);

      MeshNode* pMeshNode4 = CreateMeshNode(shader_program, mesh_data, texture_id);
      pMeshNode4->AddTransformComponent(trans);
      pMeshNode3->AddChild(pMeshNode4);
   }
   

   VrHandler::Hand hands[2] = { VrHandler::LEFT, VrHandler::RIGHT };

   for (int i = 0; i<2; i++)
   {
      gManipulator[i] = new VrManipulator(w, h, hands[i]);
      gManipulator[i]->SetWidgetScale(0.5f);
      gRootNode->AddChild(gManipulator[i]);
   }

   for (int i = 0; i<2; i++)
   {
      VrPointer* pointer = CreatePointer(hands[i]);
      gRootNode->AddChild(pointer);
   }

   for (int i = 0; i<2; i++)
   {
      VrTeleporter* teleporter = CreateWideTeleporter(hands[i]);
      gRootNode->AddChild(teleporter);
   }
   

   gRootNode->Init();
}

void VrScene::drawGui()
{
   ImGui::Image(ImTextureID(gPicker->GetPickTex(0)), ImVec2(256, 256));
   ImGui::SameLine();
   ImGui::Image(ImTextureID(gPicker->GetPickTex(1)), ImVec2(256, 256));
   ImGui::EndFrame();
}

void VrScene::updateScene()
{
   gRootNode->Tick(1.0f / 60.0f);
}

void VrScene::drawScene(const glm::mat4& P, const glm::mat4& V)
{
   lockShader();
   RenderSceneNode::P = P; //Set these early because pShadowMapPass->PreRender() needs them
   RenderSceneNode::V = V;
   pShadowMapPass->PreRender();
   drawScenePass(pShadowMapPass->GetP_light(), pShadowMapPass->GetV_light(), ShadowPass);
   pShadowMapPass->PostRender();
   unlockShader();

   drawScenePass(P, V, DisplayPass);
}

void VrScene::drawScenePass(const glm::mat4& P, const glm::mat4& V, int pass = DisplayPass)
{
   //Reset some basic state //TODO do this in root node
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);

   gRootNode->Render(P, V, pass);

   if(shaderLock==false)
   {
      glm::mat4 M = glm::mat4(1.0f);
      Draw3dGui(P, V*getBodyToWorldMatrix(), M);
   }
}

void VrScene::draw3DGuiTextures()
{
   ::draw3DGuiTextures();
}

void VrScene::keyboardScene()
{
   // WASD keyboard movement
   const float cameraMoveSpeed = 0.01f;
   glm::vec3 t;
   if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W)) 
   { 
      t += glm::vec3(headToWorldMatrix * glm::vec4(0, 0, -cameraMoveSpeed, 0)); 
   }
   if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S)) 
   { 
      t += glm::vec3(headToWorldMatrix * glm::vec4(0, 0, +cameraMoveSpeed, 0)); 
   }
   if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A)) 
   { 
      t += glm::vec3(headToWorldMatrix * glm::vec4(-cameraMoveSpeed, 0, 0, 0)); 
   }
   if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D)) 
   { 
      t += glm::vec3(headToWorldMatrix * glm::vec4(+cameraMoveSpeed, 0, 0, 0)); 
   }
   if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_C)) 
   { 
      t.y -= cameraMoveSpeed; 
   }
   if ((GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE)) || (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Z))) 
   { 
      t.y += cameraMoveSpeed; 
   }

   RenderSceneNode::Mbody = glm::translate(t)*RenderSceneNode::Mbody;
   VrHandler::BodyMotionEvent(RenderSceneNode::Mbody);
   
}

void VrScene::mouseScene()
{
   static bool inDrag = false;
   const float cameraTurnSpeed = 0.005f;
   if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
   {
      static double startX, startY;
      double currentX, currentY;
      float rx=0.0f, ry=0.0f;
      glfwGetCursorPos(window, &currentX, &currentY);
      if (inDrag)
      {
         ry = -float(currentX - startX) * cameraTurnSpeed;
         rx = -float(currentY - startY) * cameraTurnSpeed;
      }
      inDrag = true; startX = currentX; startY = currentY;

      RenderSceneNode::Mbody = glm::rotate(ry, glm::vec3(0.0f, 1.0f, 0.0f))*glm::rotate(rx, glm::vec3(1.0f, 0.0f, 0.0f))*RenderSceneNode::Mbody;
      VrHandler::BodyMotionEvent(RenderSceneNode::Mbody);
   }
   else
   {
      inDrag = false;
   }
}