#include "../Header/processPrep.h"
#ifdef WIN32
#include <windows.h>
#endif

bool Process::reload()
{
#ifdef WIN32
	SetCurrentDirectory(hge->Resource_MakePath(""));
#endif

	BGLayer::Init(tex);
	Enemy::ClearAll();
//	Ghost::ClearAll();
	Target::ClearAll();
	SelectSystem::Init();
	Effectsys::ClearAll();
	Chat::chatitem.Clear();
	Player::Init();
	EventZone::Clear();

	frameskip = M_DEFAULT_FRAMESKIP;
	strcpy(rpyfilename, "");
	Replay::rpy->InitReplayIndex();
	pauseinit = false;
	replaymode = false;
	replayFPS = 0;
	matchmode = 0;
	scene = 0;
	for (int i=0; i<M_PL_MATCHMAXPLAYER; i++)
	{
		SetShake(i, 0, true);
/*
		if (rendertar[i])
		{
			hge->Target_Free(rendertar[i]);
		}*/

		if (sprendertar[i])
		{
			delete sprendertar[i];
			sprendertar[i] = NULL;
		}
/*		rendertar[i] = hge->Target_Create(M_CLIENT_WIDTH, M_CLIENT_HEIGHT, false);*/
	}
	GameAI::Init();
	Bullet::Init(tex[TEX_BULLET]);
	Enemy::Init(tex);
	Item::Init();
	Beam::Init();
	PlayerBullet::Init(tex);
	SpriteItemManager::Init(tex);
	EffectSp::Init();

	FrontDisplay::fdisp.Init();
	Fontsys::Init(FrontDisplay::fdisp.info.normalfont);
	Fontsys::HeatUp();
	Replay::ReleaseEnumReplay();

	Replay::ReleaseList();

	GameInput::SwapInput(false);

#ifdef WIN32
	SetCurrentDirectory(hge->Resource_MakePath(""));
#endif

	return true;
}