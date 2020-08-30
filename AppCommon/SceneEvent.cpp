#include "SceneEvent.h"

void SceneEventHandler::HandleEvent(SceneEvent::EventId id)
{
   SceneEvent e(id);
   GetInstance().Notify(e);
}

void SceneEventHandler::HandleEvent(SceneEvent::EventId id, float aux)
{
   SceneEvent e(id);
   e.mAux = aux;
   GetInstance().Notify(e);
}
