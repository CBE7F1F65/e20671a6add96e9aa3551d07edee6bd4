#ifndef _PROCESS_H
#define _PROCESS_H

#include "Main.h"
#include "Export.h"
#include "Const.h"
#include "ProcessDefine.h"
#include "../../../src/hge/HGEExport.h"

class Process
{
public:
	Process();
	~Process();

	void	Release();

	bool	reload();

	int		getInput();
	char	getInputNowChar(bool wide);

	int		processStart();
	int		processPause();
	int		processContinue();
	int		processClear();
	int		processEnding();
	int		processTitle();
	int		processMatchSelect();
	int		processPlayerSelect();
	int		processSceneSelect();
	int		processOver();
	int		processSpell();
	int		processReplay();
	int		processResult();
	int		processOption();
	int		processMusic();
	int		processInit();
	int		processPreInitial();

	int		frame();
	int		render();
	void	_Render(BYTE renderflag=M_RENDER_NULL);
	void	_RenderTar();
	int		renderInit();

	void	startPrep(bool callinit = true);
	void	clearPrep(bool bclearkey = true);

	void	frameStart();
	void	frameEnd();

	void	playSE(const char * sample_name,int pan = 0);
	void	musicChange(int ID=-1, bool force = false);
	void	musicSlide(float slidetime, int tovol = 0, int pan = -101, float pitch = -1);

	void	SnapShot();

	void	SetShake(BYTE playerindex, BYTE round, bool force=false);
	void	WorldShake();

	void	SyncInput();
	void	SetState(int state, int time = 0);
	void	SetScene(BYTE scene);
	void	SetReturnValue(int retval);
	void	SetInputSwap();
	void	SetLastMatchChara(BYTE playerindex, WORD ID, WORD ID_sub_1=0xffff, WORD ID_sub_2=0xffff);

	int		AccessIP();
	bool	SetLatency(int latency);
	void	SetMatchMode(BYTE mode);
	BYTE	GetMatchMode();
	bool	IsInGame();

	void	SetStop(DWORD stopflag, int stoptime);
	DWORD	GetStopFlag(int index=-1);

	void	ClearAll();


public:
	union{
		struct{
			int keyUp;
			int keyDown;
			int keyLeft;
			int keyRight;
			int keyFire;
			int keyQuick;
			int keySlow;
			int keyDrain;
			int keyPause;
			int keySkip;
			int keyEnter;
			int keyEscape;
			int keyCapture;
		}		keyKS[M_PL_MATCHMAXPLAYER];
		int		keyKey[M_PL_MATCHMAXPLAYER][13];
	};
	union{
		struct{
			int	joyFire;
			int	joyQuick;
			int	joySlow;
			int	joyDrain;
			int	joyPause;
			int debug_joySpeedUp;
		}		keyJS[M_PL_MATCHMAXPLAYER];
		int		joyKey[M_PL_MATCHMAXPLAYER][6];
	};

	//texture
	HTEXTURE	tex[TEXMAX];

	//read ini
	char	username[M_PL_MATCHMAXPLAYER][RPYINFO_USERNAMEMAX];
	int		screenmode;
	int		bgmvol;
	int		sevol;
	int		lastmatchchara[M_PL_MATCHMAXPLAYER][M_PL_ONESETPLAYER];

	//replay
	char	rpyfilename[M_PATHMAX];
	DWORD	seed;
	int		framecounter;
	int		frameskip;
	int		renderskip;
	bool	replaymode;
	bool	replayend;
	//playtime
	LONGLONG	playtimeStart;
	bool		playing;
	//music
	HSTREAM		stream;
	HCHANNEL	channel;
	int			musicID;
	hgeChannelSyncInfo channelsyncinfo;

	//shake
	int worldshaketimer[M_PL_MATCHMAXPLAYER];
	BYTE worldshakeround[M_PL_MATCHMAXPLAYER];
	float worldx[M_PL_MATCHMAXPLAYER];
	float worldy[M_PL_MATCHMAXPLAYER];
	float worldz[M_PL_MATCHMAXPLAYER];

	//main
	int lasttime;
	int alltime;
	bool	active;

	DWORD	stopflag[FRAME_STOPINFOMAX];
	int		stoptimer[FRAME_STOPINFOMAX];

	int retvalue;


	//pause return
	bool	pauseinit;

	//Replayinfo
	float	replayFPS;

	//error
	int	errorcode;

	//indicator
	BYTE	titleselect;
	BYTE	state;
	BYTE	scene;

	//Init
	HTEXTURE	texInit;

	//
	int		latency;
	BYTE	matchmode;
	//
/*	HTARGET	rendertar[M_PL_MATCHMAXPLAYER];*/
	hgeSprite * sprendertar[M_PL_MATCHMAXPLAYER];

	static Process mp;
};

#endif