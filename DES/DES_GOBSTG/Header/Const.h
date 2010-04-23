#ifndef _CONST_H
#define _CONST_H

//#include "../../../src/hge/hgeDefines.h"

#define GAME_VERSION		0x073
#define GAME_VERSION_STR	"ver.0x073"
#define GAME_VERSION_SUB		0.98
#define GAME_VERSION_SUB_STR	"-0.98"
#define GAME_SIGNATURE		"h5nc"
#define GAME_TITLE			"–|·½»¨Ó³‰V Phantasmagoria of Flower View."
#define GAME_TITLE_STR		GAME_TITLE " " GAME_SIGNATURE " " GAME_VERSION_STR " " GAME_VERSION_SUB_STR

#define GAMEVERSION_CORE		0x08
#define GAMEVERSION_ALGORITHM	0x04
#define GAMEVERSION_RESOURCE	0x00

#define ARC(X)		((X) * 0.0001745329251994329f)
#define ANGLE(X)	(int)((X) * 5729.577951308232f)

#define DIST(X1, Y1, X2, Y2)	(sqrtf(((X1)-(X2))*((X1)-(X2)) + ((Y1)-(Y2))*((Y1)-(Y2))))
#define DIST2(X1, Y1, X2, Y2)	(((X1)-(X2))*((X1)-(X2)) + ((Y1)-(Y2))*((Y1)-(Y2)))
#define SIGN(X)					((((X)&1)<<1)-1)
#define ROLL(X, T)				(((X)%(2*(T)))/(T)*(T) - SIGN((X)%(2*(T))/(T)) * ((X)%(T)))
#define INTER(A, B, X)			(((B)-(A))*(X)+(A))
#define RANDA					((LONG)(randt()) * 36000 / RAND_MAX)

/*
//////////ROTATIONX//////////
//y = y * cosf(rotx) - z * sinf(rotx);
//z = y * sinf(rotx) + z * cosf(rotx);
//////////ROTATIONY//////////
//x = x * cosf(roty) - z * sinf(roty);
//z = x * sinf(roty) + z * cosf(roty);
//////////ROTATIONZ//////////
//x = x * cosf(rotz) - y * sinf(rotz);
//y = x * sinf(rotz) + y * cosf(rotz);
*/
#define ROTATION1(F1, F2, A)	((F1) * cost(A) - (F2) * sint(A))
#define ROTATION2(F1, F2, A)	((F1) * sint(A) + (F2) * cost(A))

#define HGELOG			hge->System_Log
#define HGELOG_ERRSTR	"**********ERROR**********"

#define TURNOFFFLAG_NONE	0x00
#define TURNOFFFLAG_LAYERSET	0x01
#define TURNOFFFLAG_EFFECTSYS	0x02

#define M_PROJECTIONMATRIX_OFFSET	0.5f

#define M_DEFAULT_FPS			60
#define M_DEFAULT_FRAMESKIP		1
#define M_DEFAULT_RENDERSKIP	1
#define M_FRAMESKIP_FASTSKIP	-3
#define M_FRAMESKIP_SLOWSKIP	2

#define M_CLIENT_LEFT			0.0f
#define M_CLIENT_RIGHT			640.0f
#define M_CLIENT_TOP			0.0f
#define M_CLIENT_BOTTOM			480.0f
#define M_CLIENT_WIDTH			(M_CLIENT_RIGHT-M_CLIENT_LEFT)
#define M_CLIENT_HEIGHT			(M_CLIENT_BOTTOM-M_CLIENT_TOP)
#define M_CLIENT_CENTER_X		((M_CLIENT_LEFT+M_CLIENT_RIGHT)/2)
#define M_CLIENT_CENTER_Y		((M_CLIENT_TOP+M_CLIENT_BOTTOM)/2)

#define M_GAMESQUARE_EDGE		16.0f
#define M_GAMESQUARE_WIDTH		(M_CLIENT_WIDTH/2-M_GAMESQUARE_EDGE*2)
#define M_GAMESQUARE_HEIGHT		(M_CLIENT_HEIGHT-M_GAMESQUARE_EDGE*2)
#define M_GAMESQUARE_TOP		(M_CLIENT_TOP+M_GAMESQUARE_EDGE)
#define M_GAMESQUARE_BOTTOM		(M_CLIENT_BOTTOM-M_GAMESQUARE_EDGE)
#define M_GAMESQUARE_LEFT_0		(M_CLIENT_LEFT+M_GAMESQUARE_EDGE)
#define M_GAMESQUARE_RIGHT_0	(M_CLIENT_CENTER_X-M_GAMESQUARE_EDGE)
#define M_GAMESQUARE_LEFT_1		(M_CLIENT_CENTER_X+M_GAMESQUARE_EDGE)
#define M_GAMESQUARE_RIGHT_1	(M_CLIENT_RIGHT-M_GAMESQUARE_EDGE)
#define	M_GAMESQUARE_LEFT_(X)	((X)?(M_GAMESQUARE_LEFT_1):(M_GAMESQUARE_LEFT_0))
#define	M_GAMESQUARE_RIGHT_(X)	((X)?(M_GAMESQUARE_RIGHT_1):(M_GAMESQUARE_RIGHT_0))
#define M_GAMESQUARE_CENTER_Y	((M_GAMESQUARE_TOP+M_GAMESQUARE_BOTTOM)/2)
#define M_GAMESQUARE_CENTER_X_0	((M_GAMESQUARE_LEFT_0+M_GAMESQUARE_RIGHT_0)/2)
#define M_GAMESQUARE_CENTER_X_1	((M_GAMESQUARE_LEFT_1+M_GAMESQUARE_RIGHT_1)/2)
#define	M_GAMESQUARE_CENTER_X_(X)	((X)?(M_GAMESQUARE_CENTER_X_1):(M_GAMESQUARE_CENTER_X_0))

#define M_GAMESQUARE_BOSSX_(X)	M_GAMESQUARE_CENTER_X_(X)
#define M_GAMESQUARE_BOSSY	(M_GAMESQUARE_CENTER_Y/2)

#define M_DELETEBOLDER			36.0f
#define M_DELETECLIENT_LEFT_(X)		(M_GAMESQUARE_LEFT_(X)-M_DELETEBOLDER)
#define M_DELETECLIENT_RIGHT_(X)	(M_GAMESQUARE_RIGHT_(X)+M_DELETEBOLDER)
#define M_DELETECLIENT_TOP		(M_GAMESQUARE_TOP-M_DELETEBOLDER)
#define M_DELETECLIENT_BOTTOM	(M_GAMESQUARE_BOTTOM+M_DELETEBOLDER)

#define M_SIDE_EDGE	(SCREEN_WIDTH/2-M_CLIENT_WIDTH*SCREEN_HEIGHT/M_CLIENT_HEIGHT/2)

#define BTYPE_BULLET	0x10
#define BTYPE_BEAM		0x20
#define BTYPE_ENEMY		0x30
#define BTYPE_GHOST		0x40

#define M_KEYKEYMAX			0x200
#define M_JOYKEYMAX			0x20

#define M_PL_CONTINUEMAX	5
#define M_PL_ONESETPLAYER	3
#define M_PL_MATCHMAXPLAYER	2

#define	M_RPYMODE_SPELL		1
#define	M_RPYMODE_PRACTICE	2

#define M_MODE_NORMAL	0
#define M_MODE_STAGE	M_RPYMODE_PRACTICE
#define M_MODE_SPELL	(M_RPYMODE_PRACTICE | M_RPYMODE_SPELL)

#define M_MATCHMODE_N2N	0x01
#define M_MATCHMODE_P2P	0x02
#define M_MATCHMODE_P2C	0x04
#define M_MATCHMODE_C2P	0x08
#define M_MATCHMODE_C2C	0x10

#define	M_LATENCY_NULL	0
#define	M_LATENCY_MIN	1
#define	M_LATENCY_MAX	9

#define M_STAGENSCENE	100

#define M_SQUARE_2			0.70710678f

#define M_BINHEADER_OFFSET	0x40

#define M_KAILLERMAXPLAYER	8

#define PLAYERTYPEMAX		0x20
#define SCENEMAX			PLAYERTYPEMAX
//#define SPRITEITEMMAX		0x1000
#define SPELLDATAMAX		PLAYERTYPEMAX
#define BULLETTYPEMAX		0x40
#define PLAYERGHOSTMAX		4
#define PLAYERGHOSTTYPEMAX	(PLAYERGHOSTMAX * PLAYERTYPEMAX * 2)
#define PLAYERBULLETTYPE		4
#define PLAYERSHOOTTYPEMAX		(PLAYERTYPEMAX * (PLAYERGHOSTMAX + 2))
#define ENEMYTYPEMAX		0x50
#define EFFECTSYSTYPEMAX	0x100
#define TEXMAX				0x300
#define TEX_WHITE		0
#define TEX_WORD		1
#define TEX_BULLET		2//7
#define TEX_GHOST		3//8
#define PACKAGEMAX			0x20
#define MUSICMAX			0x40
#define SEMAX				0x80
#define STRINGDESCMAX		0x400
#define M_SAVEINPUTMAX		0x3600
#define M_SCRIPTFOLDERMAX	8

#define DEFAULT_RESOURCEPATH	"Resource"
/*

#ifdef __RELEASE
	#define DEFAULT_HGEDLLPATH		"Resource/hge.dll"
#else
	#ifdef _DEBUG
		#define DEFAULT_HGEDLLPATH		"../../Resource/Debug/hge_d.dll"
	#else
		#define DEFAULT_HGEDLLPATH		"../../Resource/hge.dll"
	#endif // _DEBUG
#endif // __RELEASE

#ifdef __RELEASE
	#define DEFAULT_KAILLERADLLPATH		"Resource/kailleraclient.dll"
#else
	#define DEFAULT_KAILLERADLLPATH		"../../Resource/kailleraclient.dll"
#endif*/


//pushtimer
#define M_PUSH_FIRST	40
#define M_PUSH_ROLLTO	32
#define M_PUSH_SKIP		5
#define M_NOPUSH_SKIP	240

//script

#define SCR_CUSTOMCONSTMAX	0x400

//
#define M_RENDER_NULL		0x00
#define M_RENDER_LEFT		0x01
#define	M_RENDER_RIGHT		0x02


#define CINT(p)		(*(int *)(p))
#define CUINT(p)	(*(DWORD *)(p))
#define CFLOAT(p)	(*(float *)(p))
#define CLONGLONG(p)	(*(LONGLONG *)(p))
#define CULONGLONG(p)	(*(QWORD *)(p))
#define CDOUBLE(p)		(*(double *)(p))

#define CINTN(p)		(*(int *)(&(p)))
#define CUINTN(p)		(*(DWORD *)(&(p)))
#define CLONGLONGN(p)	(*(LONGLONG *)(&(p)))
#define CULONGLONGN(p)	(*(QWORD *)(&(p)))
#define CFLOATN(p)		(*(float *)(&(p)))
#define CDOUBLEN(p)		(*(double *)(&(p)))

#endif