#ifdef WIN32
//#include <vld.h>
#endif

#include "../Header/processPrep.h"
#include "../Header/GameInput.h"

Process Process::mp;

Process::Process()
{
	active		= false;
	stream		= NULL;
	channel		= NULL;
	latency		= M_LATENCY_NULL;
	matchmode	= 0;
	ZeroMemory(&channelsyncinfo, sizeof(hgeChannelSyncInfo));
	retvalue	= PGO;
	errorcode	= PROC_ERROR_INIFILE;
	ZeroMemory(texinfo, sizeof(hgeTextureInfo) * TEXMAX);

	musicID = -1;
	screenmode = 0;
	turnoffflag = 0;

	for (int i=0; i<M_PL_MATCHMAXPLAYER; i++)
	{
/*		rendertar[i] = NULL;*/
		sprendertar[i] = NULL;
		SetShake(i, 0, true);
	}

	texInit = NULL;
}

Process::~Process()
{
}

void Process::Release()
{
	GameInput::ReleaseInput();
	if (!errorcode)
	{

		hge->Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLMUSIC, bgmvol);
		hge->Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLSE, sevol);

		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, screenmode);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_TURNOFFFLAG, turnoffflag);

		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_1_1, lastmatchchara[0][0]);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_1_2, lastmatchchara[0][1]);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_1_3, lastmatchchara[0][2]);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_2_1, lastmatchchara[1][0]);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_2_2, lastmatchchara[1][1]);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_2_3, lastmatchchara[1][2]);

		if (!Data::data.bin.empty())
		{
			Data::data.SaveBin();
		}
	}

	//
	Fontsys::Release();
	SelectSystem::ClearAll();
	Bullet::Release();
	PlayerBullet::Release();
	Item::Release();
	Chat::chatitem.Release();
	EffectSp::Release();
	FrontDisplay::fdisp.Release();
	Enemy::Release();
	SpriteItemManager::Release();
	Effectsys::Release();
	hgeEffectSystem::Release();
	Replay::ReleaseList();

	BResource::Release();
	Replay::Release();

	for (int i=0; i<M_PL_MATCHMAXPLAYER; i++)
	{
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
	}
	for(int i=0;i<TEXMAX;i++)
	{
		if(texinfo[i].tex)
			hge->Texture_Free(texinfo[i].tex);
		texinfo[i].tex = NULL;
	}
	if (texInit)
	{
		hge->Texture_Free(texInit);
		texInit = NULL;
	}
	hge->Stream_Free(stream);
	NGE_Quit();
}

void Process::ClearAll()
{
	SelectSystem::ClearAll();
	Effectsys::ClearAll();
	BGLayer::Init();
	Enemy::ClearAll();
//	Ghost::ClearAll();
	Target::ClearAll();
	Bullet::ClearItem();
	Item::ClearItem();
	PlayerBullet::ClearItem();
	Beam::ClearItem();
	Chat::chatitem.Clear();
	EffectSp::ClearItem();
	EventZone::Clear();
	GameAI::ClearAll();
	SpriteItemManager::FreeFrontSprite();
	for (int i=0; i<FRAME_STOPINFOMAX; i++)
	{
		stopflag[i] = 0;
		stoptimer[i] = 0;
	}
	for (int i=0; i<M_PL_MATCHMAXPLAYER; i++)
	{
		SetShake(i, 0, true);
	}
	replayFPS = 0;

//	matchmode = 0;
	SetInputSwap();

	pauseinit = false;
	frameskip = M_DEFAULT_FRAMESKIP;
}


void Process::musicSlide(float slidetime, int tovol, int pan, float pitch)
{
	if (hge->Channel_IsPlaying(channel))
	{
		hge->Channel_SlideTo(channel, slidetime, tovol < 0 ? bgmvol : tovol, pan, pitch);
	}
}

void Process::SetShake(BYTE playerindex, BYTE round, bool force/* =false */)
{
	if (force || worldshaketimer[playerindex] == 0)
	{
		worldshakeround[playerindex] = round;
		worldshaketimer[playerindex] = worldshakeround[playerindex] * 15;
		worldx[playerindex] = 0;
		worldy[playerindex] = 0;
		worldz[playerindex] = 0;
	}
}

void Process::WorldShake()
{
	for (int i=0; i<M_PL_MATCHMAXPLAYER; i++)
	{
		DWORD stopflag = mp.GetStopFlag();
		bool binstop = FRAME_STOPFLAGCHECK_PLAYERINDEX_(stopflag, i, FRAME_STOPFLAG_WORLDSHAKE);
		if (!binstop)
		{
			if (worldshaketimer[i])
			{
				worldshaketimer[i]--;
				if (!worldshaketimer[i])
				{
					worldshakeround[i] = 0;
					worldx[i] = 0;
					worldy[i] = 0;
				}
				else
				{
					int tangle = (worldshakeround[i]*15-worldshaketimer[i]) * 4800;
					worldx[i] = sint(tangle) * 10;
					worldy[i] = cost(tangle) * 10;
				}
			}
		}
	}
}

bool Process::LoadTextureSet(int texset/* =-1 */)
{
	bool bret = true;
	for (int i=0; i<TEXMAX; i++)
	{
		if (!texinfo[i].tex && BResource::pbres->texturedata[i].texset > 0)
		{
			if (texset < 0 || BResource::pbres->texturedata[i].texset == texset)
			{
				texinfo[i].tex = BResource::pbres->LoadTexture(i);
				if (!texinfo[i].tex && bret)
				{
					bret = false;
				}
				texinfo[i].texw = hge->Texture_GetWidth(i);
				texinfo[i].texh = hge->Texture_GetHeight(i);
			}
		}
	}
	return bret;
}

bool Process::FreeTextureSet(int texset/* =-1 */)
{
	for (int i=TEX_WHITE+1; i<TEXMAX; i++)
	{
		if (texinfo[i].tex)
		{
			if (texset < 0 || BResource::pbres->texturedata[i].texset == texset)
			{
				hge->Texture_Free(texinfo[i].tex);
				texinfo[i].tex = NULL;
			}
		}
	}
	return true;
}

void Process::musicChange(int ID, bool force)
{
	if (ID < 0)
	{
		hge->Channel_RemoveLoop(channel, &channelsyncinfo);
		ZeroMemory(&channelsyncinfo, sizeof(hgeChannelSyncInfo));
		hge->Channel_Stop(channel);
		musicID = -1;
		return;
	}
	if(!hge->Channel_IsPlaying(channel) || musicID != ID || force)
	{
		if (musicID < 0 || strcmp(BResource::pbres->musdata[ID].musicfilename, BResource::pbres->musdata[musicID].musicfilename))
		{
			if(stream)
				hge->Stream_Free(stream);
			stream = hge->Stream_Load(BResource::pbres->musdata[ID].musicfilename, 0, false);
		}
		musicID = ID;
		channelsyncinfo.startPos = BResource::pbres->musdata[musicID].startpos;
		channelsyncinfo.introLength = BResource::pbres->musdata[musicID].introlength;
		channelsyncinfo.allLength = BResource::pbres->musdata[musicID].alllength;
		if (channel)
		{
			musicSlide(0, bgmvol);
			hge->Channel_Stop(channel);
		}
		if (IsInGame())
		{
			FrontDisplay::fdisp.OnChangeMusic(musicID);
		}
		if (!stream)
		{
			return;
		}
		channel = hge->Stream_Play(stream, true, bgmvol);
		hge->Channel_SetPitch(channel, 1.0f);
		hge->Channel_SetLoop(channel, &channelsyncinfo);
		hge->Channel_SetStartPos(channel, &channelsyncinfo);
	}
}

void Process::SnapShot()
{
	hge->System_Snapshot();
}

char Process::getInputNowChar(bool wide)
{
	return 0;
}

void Process::SetState(int _state, int _time /* = 0 */)
{
	state = _state;
	if (_time >= 0)
	{
		gametime = _time;
	}
}

void Process::SetScene(BYTE _scene)
{
	scene = _scene;
}

void Process::SetReturnValue(int _retval)
{
	retvalue = _retval;
}

int Process::AccessIP()
{
	DWORD ipx;
	WORD ipport;
	Export::GetLastIP(&ipx, &ipport);
	//TODO:
	return 3;
}

bool Process::SetLatency(int _latency)
{
	latency = _latency;
	if (latency < M_LATENCY_MIN || latency > M_LATENCY_MAX)
	{
		return false;
	}
	return true;
}

void Process::SetMatchMode(BYTE mode)
{
	matchmode = mode;
}

BYTE Process::GetMatchMode()
{
	return matchmode;
}

bool Process::IsInGame()
{
	if (state == STATE_START ||
		state == STATE_PAUSE ||
		state == STATE_CONTINUE ||
		state == STATE_CLEAR)
	{
		return true;
	}
	return false;
}

void Process::SetStop(DWORD _stopflag, int _stoptime)
{
	int useindex = 0;
	int minstoptimer = 0;
	for (int i=0; i<FRAME_STOPINFOMAX; i++)
	{
		if (!stopflag[i])
		{
			useindex = i;
			break;
		}
		if (!minstoptimer || stoptimer[i] < minstoptimer)
		{
			useindex = i;
		}
	}
	stopflag[useindex] = _stopflag;
	stoptimer[useindex] = _stoptime;
}

DWORD Process::GetStopFlag(int index)
{
	if (index >= 0 && index < FRAME_STOPINFOMAX)
	{
		if (stoptimer[index])
		{
			return stopflag[index];
		}
		return 0;
	}
	DWORD _stopflag = 0;
	for (int i=0; i<FRAME_STOPINFOMAX; i++)
	{
		if (stoptimer[i])
		{
			_stopflag |= stopflag[i];
		}
	}
	return _stopflag;	
}

void Process::SetLastMatchChara(BYTE playerindex, WORD ID, WORD ID_sub_1, WORD ID_sub_2)
{
	lastmatchchara[playerindex][0] = ID;
	lastmatchchara[playerindex][1] = ID_sub_1;
	lastmatchchara[playerindex][2] = ID_sub_2;
}
