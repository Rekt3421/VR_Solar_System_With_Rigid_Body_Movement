#pragma once

#include <string>
#include "SceneNode.h"
#include "Controller.h"
#include "Observer.h"
#include "imgui_impl_vr.h"


class ControllerUiHand
{
   private:

      int mSelectedColumn;
      std::vector<int> mSelectedRow;

      int mNumColumns;
      std::vector<int> mNumRows;

      int mCurrentColumn;
      std::vector<int> mCurrentRow;

      void BeginColumns();
      bool ColumnButton(const char* label, const ImVec2& size_arg);
      void EndColumns();

      void BeginRows();
      bool RowButton(const char* label, const ImVec2& size_arg);
      bool RowCheckbox(const char* label, bool* v);
      void EndRows();

      float mScrollX;
      float mScrollY;

   public:
      ControllerUiHand();
      void Click(const AxisClickEvent& e);

   friend class ControllerUi;
};

class ControllerUi : public SceneNode, public Observer<AxisClickEvent>
{
   private:

      ControllerUiHand mHandUi[2];
      void DrawGui2d(Controller::Hand hand);

      bool mFreehandPick[2];
      bool mWidgetPick[2];
      void SetFreehand(Controller::Hand hand, bool freehand);


   public:
      ControllerUi();
      void Update(const AxisClickEvent& e);
      void DrawGui2d();
   

      static ControllerUi& GetInstance()
      {
         static ControllerUi instance;
         return instance;
      }
};
