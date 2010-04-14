#ifndef _HGEEXPORT_H
#define _HGEEXPORT_H

#include "hgeDefines.h"
extern "C"
{
#include "../nge_define.h"
#include "../helper/dictionary.h"
};

#define ZLIB_USEPSW

class HGEExport
{
public:
	HGEExport();
	~HGEExport();

	void	CALL	Init(const char * apppath);
	void	CALL	Release() ;	

	char*	CALL	System_GetErrorMessage();		
	void	CALL	System_Log(const char *format, ...) ;
	void	CALL	System_Snapshot(const char *filename=0) ;	

	float	CALL	System_Transform3DPoint(hge3DPoint * pt, hge3DPoint *ptfar=NULL){return System_Transform3DPoint(pt->x, pt->y, pt->z, ptfar);};	
	float	CALL	System_Transform3DPoint(float &x, float &y, float &z, hge3DPoint *ptfar=NULL) ;	
						
private:						
	void	CALL	System_SetStateBool  (hgeBoolState   state, bool        value) ;	
	void	CALL	System_SetStateInt   (hgeIntState    state, int         value) ;		
	void	CALL	System_SetStateString(hgeStringState state, const char *value) ;		
	bool	CALL	System_GetStateBool  (hgeBoolState   state) ;
	int		CALL	System_GetStateInt   (hgeIntState    state) ;	
	const char*	CALL	System_GetStateString(hgeStringState state) ;			
public:						
	inline void			CALL		System_SetState(hgeBoolState   state, bool        value) { System_SetStateBool  (state, value); }
	inline void			CALL		System_SetState(hgeIntState    state, int         value) { System_SetStateInt   (state, value); }
	inline void			CALL		System_SetState(hgeStringState state, const char *value) { System_SetStateString(state, value); }
	inline bool			CALL		System_GetState(hgeBoolState   state) { return System_GetStateBool  (state); }
	inline int			CALL		System_GetState(hgeIntState    state) { return System_GetStateInt   (state); }
	inline const char*	CALL		System_GetState(hgeStringState state) { return System_GetStateString(state); }		
									
	BYTE*	CALL	Resource_Load(const char *filename, DWORD *size=0) ;		
	void	CALL	Resource_Free(void *res) ;					
	bool	CALL	Resource_AttachPack(const char *filename, int password=0) ;		
	void	CALL	Resource_RemovePack(const char *filename) ;		
	void	CALL	Resource_RemoveAllPacks() ;					
	char*	CALL	Resource_SetPath(const char *filename) ;				
	char*	CALL	Resource_MakePath(const char *filename) ;		
	char*	CALL	Resource_EnumFiles(const char *wildcard=0) ;		
	char*	CALL	Resource_EnumFolders(const char *wildcard=0) ;	
	bool	CALL	Resource_CreatePack(const char * filename, int password, hgeMemoryFile * first, ...) ;		
	bool	CALL	Resource_AddFileInPack(const char * filename, int password, hgeMemoryFile * memfile) ;		
	DWORD	CALL	Resource_GetCRC(const BYTE * content, DWORD size) ;		
#ifdef ZLIB_USEPSW						
	 int	CALL	Resource_GetPSW(int psw) ;	
#else						
	char*	CALL	Resource_GetPSW(int psw) ;		
#endif // ZLIB_USEPSW					
	char*	CALL	Resource_GetPackFirstFileName(const char * packfilename) ;		
	// end					
						
	void	CALL		Ini_SetInt(const char *section, const char *name, int value) ;	
	int		CALL		Ini_GetInt(const char *section, const char *name, int def_val) ;
	void	CALL		Ini_SetFloat(const char *section, const char *name, float value) ;	
	float	CALL		Ini_GetFloat(const char *section, const char *name, float def_val) ;	
	void	CALL		Ini_SetString(const char *section, const char *name, const char *value) ;	
	char*	CALL		Ini_GetString(const char *section, const char *name, const char *def_val) ;	
						
	int		CALL		Random_Seed(int seed=0) ;	
	int		CALL		Random_Int(int minval, int maxval, bool bSelf=false);
	float	CALL		Random_Float(float minval, float maxval, bool bSelf=false);
						
	float	CALL		Timer_GetTime() ;		
	float	CALL		Timer_GetDelta() ;		
	float	CALL		Timer_GetFPS(int mod = 0) ;		
	float	CALL		Timer_GetWorstFPS(int mod) ;		
	LONGLONG	CALL	Timer_GetCurrentSystemTime();
	//end					
						
	HEFFECT		CALL	Effect_Load(const char *filename, DWORD size=0) ;		
	void		CALL	Effect_Free(HEFFECT eff) ;		
	HCHANNEL	CALL	Effect_Play(HEFFECT eff) ;			
	HCHANNEL	CALL	Effect_PlayEx(HEFFECT eff, int volume=100, int pan=0, float pitch=1.0f, bool loop=false) ;			
			
	HSTREAM		CALL	Stream_Load(const char *filename, DWORD size=0, bool bLoad=true) ;		
	void		CALL	Stream_Free(HSTREAM stream) ;		
	HCHANNEL	CALL	Stream_Play(HSTREAM stream, bool loop, int volume = 100) ;			
						
	void	CALL	Channel_SetPanning(HCHANNEL chn, int pan) ;		
	void	CALL	Channel_SetVolume(HCHANNEL chn, int volume) ;		
	void	CALL	Channel_SetPitch(HCHANNEL chn, float pitch) ;		
	void	CALL	Channel_Pause(HCHANNEL chn) ;		
	void	CALL	Channel_Resume(HCHANNEL chn) ;		
	void	CALL	Channel_Stop(HCHANNEL chn) ;		
	void	CALL	Channel_PauseAll() ;		
	void	CALL	Channel_ResumeAll() ;		
	void	CALL	Channel_StopAll() ;		
	bool	CALL	Channel_IsPlaying(HCHANNEL chn) ;		
	QWORD	CALL	Channel_GetLength(HCHANNEL chn) ;		
	QWORD	CALL	Channel_GetPos(HCHANNEL chn) ;		
	void	CALL	Channel_SetPos(HCHANNEL chn, QWORD pos) ;					
	void	CALL	Channel_SetStartPos(HCHANNEL chn, hgeChannelSyncInfo * pcsi) ;		
	void	CALL	Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan = -101, float pitch = -1) ;		
	bool	CALL	Channel_IsSliding(HCHANNEL channel) ;			
	void	CALL	Channel_SetLoop(HCHANNEL channel, hgeChannelSyncInfo * pcsi) ;		
	void	CALL	Channel_RemoveLoop(HCHANNEL channel, hgeChannelSyncInfo * pcsi) ;
						
#ifdef WIN32
	void	CALL		_Input_TranslateKey(int * key);
#endif
	bool	CALL		Input_UpdateDI();
	bool	CALL		Input_GetDIKey(int key, BYTE stateType = DIKEY_PRESSED) ;
	bool	CALL		Input_SetDIKey(int key, bool set = true) ;
	
	void	CALL		Gfx_Clear(DWORD color) ;		
	void	CALL		Gfx_RenderLine(float x1, float y1, float x2, float y2, DWORD color=0xFFFFFFFF, float z=0) ;		
	void	CALL		Gfx_RenderTriple(const hgeTriple *triple) ;		
	void	CALL		Gfx_RenderQuad(const hgeQuad *quad) ;
	void	CALL		Gfx_SetClipping(int x=0, int y=0, int w=0, int h=0) ;		
	void	CALL		Gfx_SetTransform(float x=0, float y=0, float dx=0, float dy=0, float rot=0, float hscale=0, float vscale=0) ;
						
	HTEXTURE	CALL	Texture_Create(int width, int height) ;			
	HTEXTURE	CALL	Texture_Load(const char *filename, DWORD size=0, bool bMipmap=false) ;			
	void	CALL	Texture_Free(HTEXTURE tex) ;		
	int		CALL	Texture_GetWidth(HTEXTURE tex, bool bOriginal=false) ;	
	int		CALL	Texture_GetHeight(HTEXTURE tex, bool bOriginal=false) ;

	HD3DFONT	CALL	Font_Load(const char * fontStyle,int height) ;
	void		CALL	Font_Free(HD3DFONT font) ;
	int			CALL	Gfx_RenderText(HD3DFONT font, const char * text, float x, float y, float w, float h, DWORD color = 0xffffffff) ;

private:
	void _PostError(char * errorstr);
	void	_SetSampleVolume(int vol);
	void	_SetStreamVolume(int vol);
	void	_SetFXVolume(int vol);


public:
	bool	bActive;
	char	szResourcePath[_MAX_PATH];
	char	szAppPath[_MAX_PATH];
	char	szTmpFilename[_MAX_PATH];

	char	szPackFirstFilename[_MAX_PATH];
	CResourceList*		res;
	CTextureList*		textures;

	int		nHGEFPS;
	bool	bDontSuspend;
	bool	b2DMode;

	int		nFXVolume;
	int		nStreamVolume;
	int		nSampleVolume;

	float	fTime;
	float	fDeltaTime;
	float	fFPS;
	int		nFrameCounter;
	int		nFrameSkip;
	int		nRenderSkip;

	char	szIniString[M_STRMAX];
	char	szError[M_STRMAX];
	char	szIniFile[_MAX_PATH];
	char	szLogFile[_MAX_PATH];
	dictionary * inidic;

	BYTE	keyState[16];
	BYTE	lastKeyState[16];
	float	lAnalogx;
	float	lAnalogy;
};
extern HGEExport _hge;
extern HGEExport * hge;


#endif