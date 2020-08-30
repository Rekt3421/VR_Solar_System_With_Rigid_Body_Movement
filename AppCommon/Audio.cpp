#include "Audio.h"
#include "LoadAssets.h"
#include "assert.h"

Audio::Audio()
{
   std::string SoundFilenames[SceneEvent::MAX_EVENT_ID];
   SoundFilenames[SceneEvent::STARTUP]                      = "start.ogg";
   SoundFilenames[SceneEvent::OBJECT_SELECTED]              = "click.flac";
   SoundFilenames[SceneEvent::CONTROLLER_MENU_LEFT_RIGHT]   = "gui1.ogg";
   SoundFilenames[SceneEvent::CONTROLLER_MENU_UP_DOWN]      = "gui2.ogg";
   SoundFilenames[SceneEvent::TELEPORT]                     = "start.ogg";
   
   char cwd_buf[2000];
   GetCurrentDirectoryA(sizeof(cwd_buf), cwd_buf);
   std::string cwd = cwd_buf;
   cwd += "\\";

   SceneEventHandler::GetInstance().AddObserver(this);

   std::string fname = LoadAssets::AudioPath + SoundFilenames[SceneEvent::STARTUP];

   bool loaded = mBuffer[SceneEvent::STARTUP].loadFromFile(fname);
   //assert(loaded);
   mSound.setBuffer(mBuffer[SceneEvent::STARTUP]);
   mSound.play();

   for(int i=1; i<SceneEvent::MAX_EVENT_ID; i++) //start at 1 since we already loaded the startup sound
   {
      if(SoundFilenames[i].length() > 0)
      {
         loaded = mBuffer[i].loadFromFile(LoadAssets::AudioPath + SoundFilenames[i]);
      }
   }
}

void Audio::Update(const SceneEvent& e)
{
   mControllerUiSound.setBuffer(mBuffer[e.mId]);
   switch(e.mId)
   {
      case SceneEvent::OBJECT_SELECTED:
         mControllerUiSound.setPitch(1.0f);
      break;

      case SceneEvent::CONTROLLER_MENU_LEFT_RIGHT:
         mControllerUiSound.setPitch(1.0f + e.mAux);
      break;

      case SceneEvent::CONTROLLER_MENU_UP_DOWN:
         mControllerUiSound.setPitch(1.0f + e.mAux);
      break;

      default:
         mControllerUiSound.setPitch(1.0f);
      break;
   }

   mControllerUiSound.play();
}