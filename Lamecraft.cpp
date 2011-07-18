#include <math.h>
#include <psptypes.h>

#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/Logger.h>
#include <Aurora/Utils/Timer.h>
#include <Aurora/System/SystemManager.h>

#include <Aurora/Graphics/Camera.h>

#include <Aurora/Utils/StateManager.h>

#include "LameCraft/StateMenu.h"
#include "LameCraft/SplashScreen.h"
#include "LameCraft/SoundManager.h"

using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora;

PSP_MODULE_INFO("Tutorial1", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU|THREAD_ATTR_USER);
//PSP_HEAP_SIZE_KB(48*1024);
PSP_HEAP_SIZE_KB(-1024);
//PSP_HEAP_SIZE_MAX();
int main()
{
	SetupCallbacks();

	scePowerSetClockFrequency(333, 333, 166);

	//initialize render manager
	RenderManager::InstancePtr()->Init();
	RenderManager::InstancePtr()->CollectPerformance(true);
	RenderManager::InstancePtr()->InitDebugFont();

	//set perspectives
	RenderManager::InstancePtr()->SetOrtho(0,0,0,0,0,0);
	
	//init and load sounds
	SoundManager::Instance()->Init();

	srand(time(NULL));

	//new state manager
	StateManager stateManager;
	stateManager.Init();

	RenderManager::InstancePtr()->SetClearColour(0xFF000000);

	//splash screens
	SplashScreen *screen = new SplashScreen("Assets/Lamecraft/genesis.png",0,0,480,272,3);
	Logger::Instance()->LogMessage("loaded genesis.png");
	screen->ShowSplash();
	delete screen;
	SplashScreen *screen2 = new SplashScreen("Assets/Lamecraft/pspteam.png",0,0,424,72,3);
	Logger::Instance()->LogMessage("loaded pspteam.png");
	screen2->ShowSplash();
	delete screen2;

	RenderManager::InstancePtr()->SetClearColour(0xFFF5B783);
	Logger::Instance()->LogMessage("created render manager");

	//new active state
	StateMenu *statePlay = new StateMenu();
	statePlay->Init();
	Logger::Instance()->LogMessage("made new stateplay");

	stateManager.ChangeState(statePlay);

	//trun
	while ( stateManager.Running() )
	{
		stateManager.HandleEvents();
		stateManager.Update();
		stateManager.Draw();
	}

	sceGuTerm();			// Terminating the Graphics System
	sceKernelExitGame();	// Quits Application

	return 0;
}
