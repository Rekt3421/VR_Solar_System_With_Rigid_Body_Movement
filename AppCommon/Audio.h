#pragma once

#include <SFML/Audio.hpp>
#include "Observer.h"
#include "SceneEvent.h"

#ifdef _WINDOWS
#ifdef _DEBUG
#   pragma comment(lib, "sfml-audio-d")
#else
#   pragma comment(lib, "sfml-audio")
#endif
#endif

class Audio : public Observer<SceneEvent>
{
   public:
      Audio();
      void Update(const SceneEvent& e);

   private:
      sf::SoundBuffer mBuffer[SceneEvent::MAX_EVENT_ID];
      sf::Sound mSound;
      sf::Sound mControllerUiSound;
      sf::Sound mWorldSound;

};
