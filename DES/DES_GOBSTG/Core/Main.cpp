#ifdef WIN32
#include <vld.h>
#endif


#include "../Header/Process.h"
#include "../Header/Fontsys.h"

#include "../Header/Main.h"
#include "../Header/GameInput.h"

int gametime = 0;

bool RenderFunc()
{

	Process::mp.render();

	return false;
}

bool FrameFunc()
{
	Process::mp.SyncInput();
	if (GameInput::GetKey(0, KSI_ESCAPE))
	{
		return true;
	}
	if(GameInput::GetKey(0, KSI_CAPTURE, DIKEY_DOWN))
	{
		Process::mp.SnapShot();
	}

	if(Process::mp.frame() == PQUIT)
	{
		return true;
	}

	return false;
}

bool ExitFunc()
{
	return true;
}

int GameStart(int seed=0)
{
#ifndef WIN32
	initExceptionHandler();
#endif
	Export::clientInitial(true);
	Export::clientAfterInitial();

	gametime = 0;
	Process::mp.state = STATE_INIT;
	if (!seed)
	{
		seed = hge->Timer_GetCurrentSystemTime();
	}
	Process::mp.seed = seed;
	srandt(Process::mp.seed);
	hge->Random_Seed(seed);
	//	Export::Random_Seed(Process::mp.seed);
	while (true)
	{
		ShowFps();
		InputProc();
		FrameFunc();
		RenderFunc();
//		BeginScene(1);
//		EndScene();
		LimitFps(60);
	}

	return 0;
}


//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int)
extern "C"
int main(int argc, char* argv[])
{
	GameStart();
	Process::mp.Release();	
	return 0;
}