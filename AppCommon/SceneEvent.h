#pragma once

#include "Observer.h"

class SceneEvent
{
public:

   enum EventId
   {
      STARTUP,
      OBJECT_SELECTED,
      CONTROLLER_MENU_LEFT_RIGHT,
      CONTROLLER_MENU_UP_DOWN,
      TELEPORT,
      MAX_EVENT_ID
   };

   EventId mId;
   float mAux;

   SceneEvent(EventId e) { mId = e; mAux = 0.0f; }
};


class SceneEventHandler : public Subject<SceneEvent>
{
public:

   static void HandleEvent(SceneEvent::EventId id);
   static void HandleEvent(SceneEvent::EventId id, float aux);

   static SceneEventHandler& GetInstance()
   {
      static SceneEventHandler instance;
      return instance;
   }
   
};

