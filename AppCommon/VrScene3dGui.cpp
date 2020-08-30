#include "VrScene3dGui.h"
#include "LoadMesh.h"
#include "imgui_impl_vr.h"
#include "ControllerUi.h"
#include "TrackedCamera.h"
#include "Picker.h"
#include "VrScene.h"
#include "RenderNode.h"
#include "Picker.h"

//every SceneNode is a PickListener
SceneNode* pSelectedNode = nullptr;

int brush_size = 3;
glm::vec4 brush_color(1.0f);

void CheckControllerPicking()
{
   if(PickHandler::GetPickedPtr(1) != nullptr)
   {
      SceneNode* p = dynamic_cast<SceneNode*>(PickHandler::GetPickedPtr(1)); //HAHA BAD JUJU
      if (p != nullptr)
      {
         pSelectedNode = p;
      }
   }
}

void SelectUnselectNode(SceneNode* node)
{
   ImGui::PushID((void*)node);
   if(pSelectedNode != node)
   {
      if (ImGui::Button("Select"))
      {
         pSelectedNode = node;
      }
   }
   else
   {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.5f, 0.5f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.6f, 0.6f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.7f, 0.7f, 1.0f));
      if (ImGui::Button("Unselect"))
      {
         pSelectedNode = nullptr;
      }
      ImGui::PopStyleColor(3);
   }
   ImGui::PopID();
}

void RecursiveTreeNodeView(SceneNode* node, const std::string& id)
{
   ImGui::PushID(id.c_str());
   if (node != nullptr)
   {
      if (node->GetNumChildren() == 0)
      {
         
         SelectUnselectNode(node);
         ImGui::SameLine();
         //Don't show arrow for leaf nodes

         ImGui::TreeAdvanceToLabelPos();
         ImGui::Text(node->GetName().c_str());
      }
      else
      {
         
         SelectUnselectNode(node);
         ImGui::SameLine();
         if (ImGui::TreeNode(node->GetName().c_str()))
         {
            for (auto it = node->GetChildBegin(); it != node->GetChildEnd(); it++)
            {
               RecursiveTreeNodeView(*it, id);
            }
            ImGui::TreePop();
         }
      }
   }
   ImGui::PopID();
}

void draw3DGuiTextures()
{
   static bool first_frame = true;


   if (ImGui_Impl_VR_IsHidden() == true)
   {
      return;
   }

   ImVec2 texsize = ImGui_Impl_VR_GetTextureSize();

   const float bkg_alpha = 1.0f;
   static int tab = 0;
   ImGui_Impl_VR_NewFrame(0);
   ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = bkg_alpha;

   ImGui::SetNextWindowSize(texsize);
   ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
   ImGui::Begin("Tab 0", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

   static int e = 0;
   const float scales[4] = { 0.25f, 0.5f, 1.0f, 1.5f };
   bool e0 = ImGui::RadioButton("Scale 0.25", &e, 0); ImGui::SameLine();
   bool e1 = ImGui::RadioButton("0.5", &e, 1); ImGui::SameLine();
   bool e2 = ImGui::RadioButton("1.0", &e, 2); ImGui::SameLine();
   bool e3 = ImGui::RadioButton("1.5", &e, 3);

   if (e0 || e1 || e2 || e3)
   {
      ImGui_Impl_VR_SetGuiScale(scales[e]);
   }

   static bool pinned = false;
   if (ImGui::Checkbox("Pin", &pinned))
   {
      ImGui_Impl_VR_SetPinned(pinned);
   }

   ImGui::Text("Framerate = %f", ImGui::GetIO().Framerate);

   ImGui::Text("Left Controller");
   ImGui::Text("Mouseover = %d", PickHandler::GetMouseoverID(0));
   ImGui::Text("Pick = %d", PickHandler::GetPickedID(0));

   ImGui::Text("Right Controller");
   ImGui::Text("Mouseover = %d", PickHandler::GetMouseoverID(1));
   ImGui::Text("Pick = %d", PickHandler::GetPickedID(1));

   if (ImGui::Button("Screenshot"))
   {
      ImGui_Impl_VR_GrabScreenshot();
   }

   static bool keyboard = false;
   if (ImGui::Checkbox("Show Keyboard", &keyboard))
   {
      if (keyboard == true)
      {
         ImGui_Impl_VR_ShowKeyboard();
      }
      else
      {
         ImGui_Impl_VR_HideKeyboard();
      }
   }
  
   ImGui::End();
   ImGui_Impl_VR_Render(0);

   ImGui_Impl_VR_NewFrame(1);

   ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = bkg_alpha;

   ImGui::SetNextWindowSize(texsize);
   ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
   ImGui::Begin("Tab 1", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

   if (ImGui::Checkbox("Pin", &pinned))
   {
      ImGui_Impl_VR_SetPinned(pinned);
   }

   static bool cameraFeed = false;
   
   if(ImGui::Checkbox("Enable camera feed", &cameraFeed))
   {
      if(cameraFeed == true)
      {
         TrackedCamera::HmdCam.StartVideoPreview();
      }
      else
      {
         TrackedCamera::HmdCam.StopVideoPreview();
      }
   }

   if (cameraFeed == true)
   {
      TrackedCamera::HmdCam.GetTrackedCameraTextureID();
      ImGui::ImageButton(ImTextureID(TrackedCamera::HmdCam.GetCameraTextureID()), ImVec2(200.f, 200.f));
   }

   ImGui::End();
   ImGui_Impl_VR_Render(1);


   ImGui_Impl_VR_NewFrame(2);
   ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = bkg_alpha;
   ImGui::SetNextWindowSize(texsize);
   ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
   ImGui::Begin("Tab 2", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
   
      ImGui::Text("Selected Node");
      CheckControllerPicking();
      if(pSelectedNode != nullptr)
      {
         RecursiveTreeNodeView(pSelectedNode, "select");
      }
      else
      {
         ImGui::Text("None");
      }
   
      ImGui::Separator();
      ImGui::Text("Operations on selected");
      if (ImGui::Button("Delete"))
      {
         if (pSelectedNode != nullptr)
         {
            pSelectedNode->GetParent()->RemoveChild(pSelectedNode);
            delete pSelectedNode;
            pSelectedNode = nullptr;
         }
      }
      ImGui::SameLine();

      static SceneNode* pCutNode = nullptr;
      if (ImGui::Button("Cut"))
      {
         pCutNode = pSelectedNode;
      }
      ImGui::SameLine();

      if (ImGui::Button("Paste"))
      {
         if(pCutNode != nullptr && pSelectedNode != nullptr)
         {
            pCutNode->GetParent()->RemoveChild(pCutNode);
            pSelectedNode->AddChild(pCutNode);
      
            pCutNode = nullptr;
         }
      }
      ImGui::SameLine();
      if (ImGui::Button("Clone"))
      {
         SceneNode* newNode = pSelectedNode->clone();
         pSelectedNode->AddChild(newNode);
      }

   
      ImGui::Separator();
      ImGui::Text("Cut Node");
      if (pSelectedNode != nullptr)
      {
         RecursiveTreeNodeView(pCutNode, "copy");
      }
      else
      {
         ImGui::Text("None");
      }

      ImGui::Separator();
      ImGui::Text("Scene graph");
      RenderSceneNode* root = VrScene::GetRootNode();
      RecursiveTreeNodeView(root, "scene");
  
   ImGui::End();
   ImGui_Impl_VR_Render(2);


   ImGui_Impl_VR_NewFrame(3);
   ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = bkg_alpha;
   ImGui::SetNextWindowSize(texsize);
   ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
   ImGui::Begin("Tab 3", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
   
      if (pSelectedNode != nullptr)
      {
         TreeViewVisitor treeview;
         pSelectedNode->Accept(treeview);
      }

   ImGui::End();
   ImGui_Impl_VR_Render(3);


   ControllerUi::GetInstance().DrawGui2d();

}