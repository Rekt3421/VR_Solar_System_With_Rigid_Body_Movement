#include <GL/glew.h>
#include "ControllerUi.h"
#include "imgui_impl_vr.h"

#include "Audio.h" //for SceneEventHandler

#include "VrScene.h"
#include "Picker.h"
#include "VrManipulator.h"

#include "VrPointer.h"
#include "VrTeleporter.h"

namespace VrScene
{
   extern Picker* gPicker;
   extern VrManipulator* gManipulator[2];
}

ControllerUiHand::ControllerUiHand():mScrollX(0), mScrollY(0)
{
   mSelectedRow.resize(10);
   mNumRows.resize(10);
   mCurrentRow.resize(10);

   mSelectedColumn = 0;
}

void ControllerUiHand::Click(const AxisClickEvent& e)
{
   if (e.mState == Controller::PRESSED)
   {
      if (e.mX*e.mX + e.mY*e.mY > 0.5f)
      {
         if (e.mX > e.mY && e.mX > -e.mY)
         {
            if(mNumColumns>0)
            {
               mSelectedColumn = (mSelectedColumn + 1) % mNumColumns;
               SceneEventHandler::HandleEvent(SceneEvent::CONTROLLER_MENU_LEFT_RIGHT, float(mSelectedColumn)/float(mNumColumns));
            }
         }
         else if (e.mX < e.mY && e.mX < -e.mY)
         {
            if (mNumColumns>0)
            {
               mSelectedColumn = (mSelectedColumn + (mNumColumns - 1)) % mNumColumns;
               SceneEventHandler::HandleEvent(SceneEvent::CONTROLLER_MENU_LEFT_RIGHT, float(mSelectedColumn) / float(mNumColumns));
            }
         }
         else if (e.mX < e.mY && -e.mX < e.mY)
         {
            int& selected_row = mSelectedRow[mCurrentColumn];
            if(mNumRows[mCurrentColumn] > 0)
            {
               selected_row = (selected_row + (mNumRows[mCurrentColumn] - 1)) % mNumRows[mCurrentColumn];
               SceneEventHandler::HandleEvent(SceneEvent::CONTROLLER_MENU_UP_DOWN, float(selected_row) / float(mNumRows[mCurrentColumn]));
            }
         }
         else if (e.mX > e.mY && -e.mX > e.mY)
         {
            int& selected_row = mSelectedRow[mCurrentColumn];
            if (mNumRows[mCurrentColumn] > 0)
            {
               selected_row = (selected_row + 1) % mNumRows[mCurrentColumn];
               SceneEventHandler::HandleEvent(SceneEvent::CONTROLLER_MENU_UP_DOWN, float(selected_row) / float(mNumRows[mCurrentColumn]));
            }
         }
      }
   }
}

void ControllerUiHand::BeginColumns()
{
   mCurrentColumn = 0;
}

bool ControllerUiHand::ColumnButton(const char* label, const ImVec2& size_arg)
{
   
   if (mCurrentColumn == mSelectedColumn)
   {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
   }
   bool ret = ImGui::Button(label, size_arg);

   if (mCurrentColumn == mSelectedColumn)
   {
      ImGui::PopStyleColor();
      mScrollX += 0.25f*(mCurrentColumn*240.0f - mScrollX);
      ImGui::SetScrollX(mScrollX);
   }
   ImGui::SameLine();
   mCurrentColumn++;

   return ret;
}

void ControllerUiHand::EndColumns()
{
   mNumColumns = mCurrentColumn;
}

void ControllerUiHand::BeginRows()
{
   mCurrentRow[mCurrentColumn] = 0;
}

bool ControllerUiHand::RowButton(const char* label, const ImVec2& size_arg)
{

   int& current_row = mCurrentRow[mCurrentColumn];
   const int selected_row = mSelectedRow[mCurrentColumn];

   if (current_row == selected_row)
   {
      ImGui::GetIO().MousePos = (ImGui::GetCursorScreenPos());
   }
   bool ret = ImGui::Button(label, size_arg);

   if (current_row == selected_row)
   {
      mScrollY += 0.25f*(current_row*50.0f - mScrollY);
      ImGui::SetScrollY(mScrollY);
   }

   current_row++;
   return ret;
}

bool ControllerUiHand::RowCheckbox(const char* label, bool* v)
{
   int& current_row = mCurrentRow[mCurrentColumn];
   const int selected_row = mSelectedRow[mCurrentColumn];

   if (current_row == selected_row)
   {
      ImGui::GetIO().MousePos = (ImGui::GetCursorScreenPos());
   }

   bool ret = ImGui::Checkbox(label, v);

   if (current_row == selected_row)
   {
      mScrollY += 0.25f*(current_row*50.0f - mScrollY);
      ImGui::SetScrollY(mScrollY);
   }

   current_row++;
   return ret;
}

void ControllerUiHand::EndRows()
{
   mNumRows[mCurrentColumn] = mCurrentRow[mCurrentColumn];
}


ControllerUi::ControllerUi()
{
   Controller::GetInstance().Subject<AxisClickEvent>::AddObserver(this);

   for (int i = 0; i < 2; i++)
   {
      mFreehandPick[i] = true;
      mWidgetPick[i] = false;
   }
}

void ControllerUi::Update(const AxisClickEvent& e)
{
   mHandUi[e.mHand].Click(e);
}

void ControllerUi::DrawGui2d()
{
   DrawGui2d(Controller::LEFT);
   DrawGui2d(Controller::RIGHT);
}

void EnableTeleport(Controller::Hand hand)
{
   VrPointer::sPointers[hand]->SetVisible(false);
   VrTeleporter::sTeleporters[hand]->SetVisible(true);
}

void DisableTeleport(Controller::Hand hand)
{
   VrPointer::sPointers[hand]->SetVisible(true);
   VrTeleporter::sTeleporters[hand]->SetVisible(false);
}

void ControllerUi::SetFreehand(Controller::Hand hand, bool freehand)
{
   mFreehandPick[hand] = freehand;
   mWidgetPick[hand] = !freehand;

   if (mFreehandPick[hand]==true)
   {
      VrScene::selectMode[hand] = VrScene::FREEHAND;

      VrScene::gManipulator[hand]->SetVisible(false);
      VrScene::gManipulator[hand]->SetRenderNode(nullptr);
   }
   else
   {
      VrScene::selectMode[hand] = VrScene::WIDGET;
   }
}

void ControllerUi::DrawGui2d(Controller::Hand hand)
{
   const ImVec2 texsize = ImGui_Impl_VR_GetTextureSize();
   const float bkg_alpha = 1.0f;

   const int context = 4+hand;
   ControllerUiHand& handUi = mHandUi[hand];

   const ImVec4 window_colors[2] = { ImVec4(1.0f, 0.0f, 0.0f, 1.0f) , ImVec4(0.0f, 1.0f, 0.0f, 1.0f) };

   ImGui_Impl_VR_NewFrame(context);
   ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = bkg_alpha;
   ImGui::PushStyleColor(ImGuiCol_TitleBg, window_colors[hand]);
   ImGui::PushStyleColor(ImGuiCol_TitleBgActive, window_colors[hand]);
   
   ImGui::SetNextWindowSize(ImVec2(texsize.x, texsize.y / 4.0f));
   ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
   ImGui::Begin("Mode", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
   ImGui::SetWindowFontScale(1.75f);

   const ImVec2 button_size(240.0f, 50.0f);
   handUi.BeginColumns();
   handUi.ColumnButton("Select", button_size);
   handUi.ColumnButton("Transform", button_size);
   handUi.ColumnButton("Move", button_size);
   handUi.EndColumns();
   ImGui::End();
   ImGui::PopStyleColor(2);

   ImGui::SetNextWindowSize(ImVec2(texsize.x / 2.0f, texsize.y / 2.0f));
   ImGui::SetNextWindowPos(ImVec2(0.0f, texsize.y / 4.0f));
   ImGui::Begin("Options", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
   ImGui::SetWindowFontScale(1.5f);
   handUi.BeginRows();

   if(handUi.mSelectedColumn==0) //Select
   {
      DisableTeleport(hand);
      if(handUi.RowCheckbox("Freehand", &mFreehandPick[hand]))
      {
         SetFreehand(hand, mFreehandPick[hand]);
      }
      if(handUi.RowCheckbox("Widget", &mWidgetPick[hand]))
      {
         SetFreehand(hand, !mWidgetPick[hand]);
      }
   }
   else if (handUi.mSelectedColumn == 1) //Transform
   {
      DisableTeleport(hand);
      bool t = VrScene::gManipulator[hand]->GetCurrentTransform() == VrManipulator::Transform::TranslateTransform;
      bool r = VrScene::gManipulator[hand]->GetCurrentTransform() == VrManipulator::Transform::RotateTransform;
      bool s = VrScene::gManipulator[hand]->GetCurrentTransform() == VrManipulator::Transform::ScaleTransform;
      if(handUi.RowCheckbox("Translate", &t))
      {
         VrScene::gManipulator[hand]->SetCurrentTransform(VrManipulator::Transform::TranslateTransform);
      }

      if (handUi.RowCheckbox("Rotate", &r))
      {
         VrScene::gManipulator[hand]->SetCurrentTransform(VrManipulator::Transform::RotateTransform);
      }

      if(handUi.RowCheckbox("Scale", &s))
      {
         VrScene::gManipulator[hand]->SetCurrentTransform(VrManipulator::Transform::ScaleTransform);
      }

      static bool snap = false;
      handUi.RowCheckbox("Snap", &snap);

   }
   else if (handUi.mSelectedColumn == 2) //Move
   {
      EnableTeleport(hand);
   }
   handUi.EndRows();
   ImGui::End();
   ImGui_Impl_VR_Render(context);
  
}
