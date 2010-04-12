#include "HGEExport.h"
#include "zlib/zip.h"
#include "zlib/unzip.h"
extern "C"
{
#include "../helper/dictionary.h"
#include "../helper/iniparser.h"
#include "../nge_timer.h"
#include "../nge_io_file.h"
#include "../nge_image_load.h"
#include "../nge_graphics.h"
};
#include <stdlib.h>

unsigned int g_seed=0;
unsigned int g_seed_self=0;

HGEExport _hge;
HGEExport * hge = &_hge;

HGEExport::HGEExport()
{
}

HGEExport::~HGEExport()
{

}

void CALL HGEExport::Init(const char * apppath)
{
	inidic = NULL;
	inidic = dictionary_new(1);
	b2DMode = true;
	bActive = true;
	nHGEFPS = -1;
	bDontSuspend = false;
	fTime = 0;
	fDeltaTime = 0;
	fFPS = 0;
	nFXVolume = 10000;
	nSampleVolume = 10000;
	nStreamVolume = 10000;
	nFrameCounter = 0;
	nFrameSkip = 0;
	nRenderSkip = 0;
	strcpy(szError, "");
	strcpy(szIniString, "");

	strcpy(szAppPath, apppath);
	int i;
	for(i=strlen(szAppPath)-1; i>0; i--) if(szAppPath[i]=='\\') break;
	szAppPath[i+1]=0;

	strcpy(szResourcePath, szAppPath);

	res = NULL;
	textures = NULL;
}

void CALL HGEExport::Release()
{
	if (inidic)
	{
		dictionary_del(inidic);
		inidic = NULL;
	}
}

char * CALL HGEExport::System_GetErrorMessage()
{
	return szError;
}

void CALL HGEExport::System_Log(const char *szFormat, ...)
{

	int ifile = NULL;
	va_list ap;

	if(!szLogFile[0]) return;

	ifile = io_fopen(szLogFile, IO_APPEND);
	if(!ifile) return;

	char message[1024];
	va_start(ap, szFormat);
	int iret = vsnprintf(message, sizeof(char)*1022, szFormat, ap);
	va_end(ap);
	message[iret] = '\n';
	message[iret+1] = 0;
	io_fwrite(message, 1, iret+2, ifile);
	io_fclose(ifile);
}

void CALL HGEExport::System_Snapshot(const char *filename/* =0 */)
{
}

float CALL HGEExport::System_Transform3DPoint(float &x, float &y, float &z, hge3DPoint *ptfar)
{
	if (!ptfar || ptfar->z == 0.0f)
	{
		return 1.0f;
	}
	float scale = (ptfar->z - z) / ptfar->z;
	x = (x - ptfar->x) * scale + ptfar->x;
	y = (y - ptfar->y) * scale + ptfar->y;
	z = 0;
	return scale;
}

void CALL HGEExport::System_SetStateBool(hgeBoolState state, bool value)
{
	switch(state)
	{
	case HGE_WINDOWED:
		break;
	case HGE_ZBUFFER:
		break;
	case HGE_TEXTUREFILTER:
		break;
	case HGE_USESOUND:
		break;

	case HGE_HIDEMOUSE:
		break;

	case HGE_DONTSUSPEND:
		bDontSuspend=value;
		break;

	case HGE_2DMODE:
		b2DMode=value;
		break;
	}
}

void CALL HGEExport::System_SetStateInt(hgeIntState state, int value)
{
	switch(state)
	{
	case HGE_SCREENWIDTH:
		break;

	case HGE_SCREENHEIGHT:
		break;

	case HGE_SCREENBPP:
		break;

	case HGE_SAMPLERATE:
		break;

	case HGE_FXVOLUME:
		nFXVolume=value;
		_SetFXVolume(nFXVolume);
		break;

	case HGE_SAMPLEVOLUME:
		nSampleVolume=value;
		_SetSampleVolume(nSampleVolume);
		break;
	case HGE_STREAMVOLUME:
		nStreamVolume=value;
		_SetStreamVolume(nStreamVolume);
		break;


	case HGE_FPS://			if(VertArray) break;
		nHGEFPS=value;
		break;

		/************************************************************************/
		/* These cases are added by h5nc (h5nc@yahoo.com.cn)                    */
		/************************************************************************/
	case HGE_FRAMESKIP:
		nFrameSkip = value;
		break;
	case HGE_RENDERSKIP:
		nRenderSkip = value;
		break;
	}
}

void CALL HGEExport::System_SetStateString(hgeStringState state, const char *value)
{
	switch(state)
	{
	case HGE_ICON:
		break;
	case HGE_TITLE:
		break;
	case HGE_INIFILE:		if(value) strcpy(szIniFile,Resource_MakePath(value));
							else szIniFile[0]=0;
							break;
	case HGE_LOGFILE:		if(value)
							{
								strcpy(szLogFile,Resource_MakePath(value));
								int ifile = io_fopen(szLogFile, IO_WRONLY);
								if(!ifile) szLogFile[0]=0;
								else io_fclose(ifile);
							}
							else szLogFile[0]=0;
							break;

	case HGE_BASSDLLFILE:
		break;
	}
}

bool CALL HGEExport::System_GetStateBool(hgeBoolState state)
{
	switch(state)
	{
	case HGE_WINDOWED:		return true;
	case HGE_ZBUFFER:		return false;
	case HGE_TEXTUREFILTER:	return false;
	case HGE_USESOUND:		return true;
	case HGE_DONTSUSPEND:	return bDontSuspend;
	case HGE_HIDEMOUSE:		return true;
	case HGE_2DMODE:		return b2DMode;
	}
	return false;
}

int CALL HGEExport::System_GetStateInt(hgeIntState state)
{
	switch(state)
	{
	case HGE_SCREENWIDTH:	return SCREEN_WIDTH;
	case HGE_SCREENHEIGHT:	return SCREEN_HEIGHT;
	case HGE_SCREENBPP:		return 32;
	case HGE_SAMPLERATE:	return 44100;
	case HGE_FXVOLUME:		return nFXVolume;
	case HGE_SAMPLEVOLUME:	return nSampleVolume;
	case HGE_STREAMVOLUME:	return nStreamVolume;
	case HGE_FPS:			return nHGEFPS;
	case HGE_FRAMECOUNTER:	return nFrameCounter;
	case HGE_FRAMESKIP:		return nFrameSkip;
	case HGE_RENDERSKIP:	return nRenderSkip;
	}

	return 0;
}

const char* CALL HGEExport::System_GetStateString(hgeStringState state) {
	switch(state) {
		case HGE_ICON:			return 0;
		case HGE_TITLE:			return 0;
		case HGE_INIFILE:		if(szIniFile[0]) return szIniFile;
								else return 0;
		case HGE_LOGFILE:		if(szLogFile[0]) return szLogFile;
								else return 0;
	}
	return NULL;
}

BYTE * CALL HGEExport::Resource_Load(const char* filename, DWORD * size)
{
	static char *res_err="Can't load resource: %s";

	CResourceList *resItem=res;
	char szName[_MAX_PATH];
	char szZipName[_MAX_PATH];
	unzFile zip;
	unz_file_info file_info;
	int done, i;
	BYTE * ptr;

	if(size)
		*size = 0;

	if(filename[0]=='\\' || filename[0]=='/' || filename[1]==':') goto _fromfile; // skip absolute paths

	// Load from pack

	strcpy(szName,filename);
	strupr(szName);
	for(i=0; szName[i]; i++) { if(szName[i]=='/') szName[i]='\\'; }

	while(resItem)
	{
		zip=unzOpen(resItem->filename);
		done=unzGoToFirstFile(zip);
		while(done==UNZ_OK)
		{
			unzGetCurrentFileInfo(zip, &file_info, szZipName, sizeof(szZipName), NULL, 0, NULL, 0);
			strupr(szZipName);
			for(i=0; szZipName[i]; i++)	{ if(szZipName[i]=='/') szZipName[i]='\\'; }
			if(!strcmp(szName,szZipName))
			{
				if(unzOpenCurrentFilePassword(zip, Resource_GetPSW(resItem->password)/* ? resItem->password : 0*/) != UNZ_OK)
				{
					unzClose(zip);
					sprintf(szName, res_err, filename);
					_PostError(szName);
					return 0;
				}

				ptr = (BYTE *)malloc(file_info.uncompressed_size);
				if(!ptr)
				{
					unzCloseCurrentFile(zip);
					unzClose(zip);
					sprintf(szName, res_err, filename);
					_PostError(szName);
					return 0;
				}

				if(unzReadCurrentFile(zip, ptr, file_info.uncompressed_size) < 0)
				{
					unzCloseCurrentFile(zip);
					unzClose(zip);
					free(ptr);
					sprintf(szName, res_err, filename);
					_PostError(szName);
					return 0;
				}
				unzCloseCurrentFile(zip);
				unzClose(zip);
				if(size) *size=file_info.uncompressed_size;
				return ptr;
			}

			done=unzGoToNextFile(zip);
		}

		unzClose(zip);
		resItem=resItem->next;
	}

	// Load from file
_fromfile:
	int ifile = io_fopen(filename, IO_RDONLY/*"rb"*/);
	if (!ifile)
	{
		return 0;
	}
	if (size)
	{
		*size = io_fsize(ifile);
	}
	BYTE * content = (BYTE *)malloc(*size*sizeof(BYTE));
	io_fread((void*)content, 1, *size*sizeof(BYTE), ifile);
	io_fclose(ifile);
	return content;
}

void CALL HGEExport::Resource_Free(void *res)
{
	if (res)
	{
		free(res);
	}
}

bool CALL HGEExport::Resource_AttachPack(const char *filename, int password)
{
	char *szName;
	CResourceList *resItem=res;
	unzFile zip;

	szName=Resource_MakePath(filename);
	strupr(szName);

	while(resItem)
	{
		if(!strcmp(szName,resItem->filename)) return true;
		resItem=resItem->next;
	}

	zip=unzOpen(szName);
	if(!zip) return false;
	unzClose(zip);

	resItem=new CResourceList;
	strcpy(resItem->filename, szName);
	if(password)
		resItem->password = password;
	else resItem->password=0;
	resItem->next=res;
	res=resItem;

	return true;
}

bool CALL HGEExport::Resource_AddFileInPack(const char * filename, int password, hgeMemoryFile * memfile)
{
	if(!Resource_AttachPack(filename, password))
	{
		return Resource_CreatePack(filename, password, memfile, NULL);
	}

	zipFile zip = zipOpen(Resource_MakePath(filename), APPEND_STATUS_ADDINZIP);

	if(!zip)
		return false;

	if(Z_OK != 
		zipOpenNewFileInZip3(
		zip, memfile->filename, NULL,
		NULL, 0, NULL, 0, NULL,
		Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0,
		-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
		Resource_GetPSW(password), Resource_GetCRC(memfile->data, memfile->size)
		))
	{
		zipClose(zip, NULL);
		return false;
	}
	if(Z_OK != zipWriteInFileInZip(zip, memfile->data, memfile->size))
	{
		zipCloseFileInZip(zip);
		zipClose(zip, NULL);
		return false;
	}

	zipCloseFileInZip(zip);
	zipClose(zip, NULL);

	return true;
}


bool CALL HGEExport::Resource_CreatePack(const char * filename, int password, hgeMemoryFile * first, ...)
{
	Resource_RemovePack(filename);

	va_list ap;
	va_start(ap, first);

	zipFile zip = zipOpen(Resource_MakePath(filename), APPEND_STATUS_CREATE);

	if(!zip)
		return false;

	hgeMemoryFile * vai = first;

	while(vai != NULL)
	{
		if(Z_OK != 
			zipOpenNewFileInZip3(
			zip, vai->filename, NULL,
			NULL, 0, NULL, 0, NULL,
			Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0,
			-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
			Resource_GetPSW(password), Resource_GetCRC(vai->data, vai->size)
			))
		{
			zipClose(zip, NULL);
			return false;
		}
		if(Z_OK != zipWriteInFileInZip(zip, vai->data, vai->size))
		{
			zipCloseFileInZip(zip);
			zipClose(zip, NULL);
			return false;
		}
		zipCloseFileInZip(zip);

		vai = va_arg(ap, hgeMemoryFile *);
	}

	zipClose(zip, NULL);

	va_end(ap);

	return Resource_AttachPack(filename, password);
}

void CALL HGEExport::Resource_RemovePack(const char *filename)
{
	char *szName;
	CResourceList *resItem=res, *resPrev=0;

	szName=Resource_MakePath(filename);
	strupr(szName);

	while(resItem)
	{
		if(!strcmp(szName,resItem->filename))
		{
			if(resPrev) resPrev->next=resItem->next;
			else res=resItem->next;
			delete resItem;
			break;
		}

		resPrev=resItem;
		resItem=resItem->next;
	}
}

void CALL HGEExport::Resource_RemoveAllPacks()
{
	CResourceList *resItem=res, *resNextItem;

	while(resItem)
	{
		resNextItem=resItem->next;
		delete resItem;
		resItem=resNextItem;
	}

	res=0;
}


/************************************************************************/
/* These functions are added by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
// begin
DWORD CALL HGEExport::Resource_GetCRC(const BYTE * content, DWORD size)
{
	return crc32(0, (Bytef *)content, size);
}

#ifdef ZLIB_USEPSW
int CALL HGEExport::Resource_GetPSW(int psw)
{
	return psw;
}
#else
char * CALL HGEExport::Resource_GetPSW(int psw)
{
	static char szPSWBuffer[20];
	strcpy(szPSWBuffer, "PSW");
	for (int i=0; i<4; i++)
	{
		psw = psw * 214013 + 2531011;
		szPSWBuffer[i*4+3] = (psw & 0xFF000000) >> 24;
		szPSWBuffer[i*4+1+3] = (psw & 0x00FF0000) >> 16;
		szPSWBuffer[i*4+2+3] = (psw & 0x0000FF00) >> 8;
		szPSWBuffer[i*4+3+3] = (psw & 0x000000FF);
	}
	szPSWBuffer[19] = 0;
	for (int i=3; i<19; i++)
	{
		if (szPSWBuffer[i] == 0)
		{
			szPSWBuffer[i] = szPSWBuffer[i-1];
		}
	}
	return szPSWBuffer;
	return (char *)psw;
}
#endif

char * CALL HGEExport::Resource_GetPackFirstFileName(const char * packfilename)
{
	unzFile zip;
	zip=unzOpen(Resource_MakePath(packfilename));
	if(UNZ_OK == unzGoToFirstFile(zip))
	{
		unzGetCurrentFileInfo(zip, NULL, szPackFirstFilename, sizeof(szPackFirstFilename), NULL, 0, NULL, 0);
		unzClose(zip);
		return szPackFirstFilename;
	}
	unzClose(zip);
	return NULL;
}
// end

// TODO:
char * CALL HGEExport::Resource_SetPath(const char * filename)
{
	int i;

	if(filename[0]=='\\' || filename[0]=='/' || filename[1]==':')
		strcpy(szResourcePath, filename);

	else
	{
		char szTmp[256];
		//		strcpy(szResourcePath, szAppPath);
		strcpy(szTmp, filename);
		while(szTmp[0] == '.' && szTmp[1] == '.')
		{
			i = strlen(szResourcePath) - 1;
			if(szResourcePath[i] == '/' || szResourcePath[i] == '\\')
				szResourcePath[i] = 0;
			for(i = strlen(szResourcePath) - 1; i >= 0; i--)
			{
				if(szResourcePath[i] != '/' && szResourcePath[i] != '\\')
					szResourcePath[i] = 0;
				else
					break;
			}
			for(i = 0; i < (int)(strlen(szTmp) - 2); i++)
			{
				szTmp[i] = szTmp[i+3];
			}
		}
		if(szTmp) strcat(szResourcePath, szTmp);
	}

	for(i=0; szResourcePath[i]; i++) { if(szResourcePath[i]=='/') szResourcePath[i]='\\'; }
	if(szResourcePath[i-1] != '\\')
	{
		szResourcePath[i] = '\\';
		szResourcePath[i+1] = 0;
	}
	return szResourcePath;
}

char * CALL HGEExport::Resource_MakePath(const char * filename)
{
	int i;

	if(!filename)
		strcpy(szTmpFilename, szAppPath);
	else if(filename[0]=='\\' || filename[0]=='/' || filename[1]==':')
		strcpy(szTmpFilename, filename);

	else
	{
		char szTmp[256];
		strcpy(szTmpFilename, szResourcePath);
		strcpy(szTmp, filename);
		while(szTmp[0] == '.' && szTmp[1] == '.')
		{
			i = strlen(szTmpFilename) - 1;
			if(szTmpFilename[i] == '/' || szTmpFilename[i] == '\\')
				szTmpFilename[i] = 0;
			for(i = strlen(szTmpFilename) - 1; i >= 0; i--)
			{
				if(szTmpFilename[i] != '/' && szTmpFilename[i] != '\\')
					szTmpFilename[i] = 0;
				else
					break;
			}
			for(i = 0; i < (int)(strlen(szTmp) - 2); i++)
			{
				szTmp[i] = szTmp[i+3];
			}
		}
		if(szTmp) strcat(szTmpFilename, szTmp);
	}

	for(i=0; szTmpFilename[i]; i++) { if(szTmpFilename[i]=='/') szTmpFilename[i]='\\'; }
	return szTmpFilename;
}

char * CALL HGEExport::Resource_EnumFiles(const char *wildcard/* =0 */)
{
	return NULL;
}

char * CALL HGEExport::Resource_EnumFolders(const char *wildcard/* =0 */)
{
	return NULL;
}

void CALL HGEExport::Ini_SetInt(const char *section, const char *name, int value)
{
	if (inidic)
	{
		char bufferstr[M_STRMAX];
		char buffer[M_STRITOAMAX];
		sprintf(bufferstr, "%s_%s", section, name);
		sprintf(buffer, "%d", value);
		iniparser_set(inidic, bufferstr, buffer);
	}
}

int CALL HGEExport::Ini_GetInt(const char *section, const char *name, int def_val)
{
	int iret = def_val;
	if (inidic)
	{
		char bufferstr[M_STRMAX];
		sprintf(bufferstr, "%s_%s", section, name);
		iret = iniparser_getint(inidic, bufferstr, def_val);
	}
	return iret;
}

void CALL HGEExport::Ini_SetFloat(const char *section, const char *name, float value)
{
	if (inidic)
	{
		char bufferstr[M_STRMAX];
		char buffer[M_STRITOAMAX];
		sprintf(bufferstr, "%s_%s", section, name);
		sprintf(buffer, "%f", value);
		iniparser_set(inidic, bufferstr, buffer);
	}
}

float CALL HGEExport::Ini_GetFloat(const char *section, const char *name, float def_val)
{
	float fret = def_val;
	if (inidic)
	{
		char bufferstr[M_STRMAX];
		char bufferdef[M_STRITOAMAX];
		char bufferret[M_STRITOAMAX];
		sprintf(bufferstr, "%s_%s", section, name);
		sprintf(bufferdef, "%f", def_val);
		strcpy(bufferret, iniparser_getstring(inidic, bufferstr, bufferdef));
		fret = (float)atof(bufferret);
	}
	return fret;
}

void CALL HGEExport::Ini_SetString(const char *section, const char *name, const char *value)
{
	if (inidic)
	{
		char bufferstr[M_STRMAX];
		sprintf(bufferstr, "%s_%s", section, name);
		iniparser_set(inidic, bufferstr, (char *)value);
	}
}

char * CALL HGEExport::Ini_GetString(const char *section, const char *name, const char *def_val)
{
	if (inidic)
	{
		char bufferstr[M_STRMAX];
		sprintf(bufferstr, "%s_%s", section, name);
		strcpy(szIniString, iniparser_getstring(inidic, bufferstr, (char *)def_val));
	}
	else
	{
		strcpy(szIniString, def_val);
	}
	return szIniString;
}

int CALL HGEExport::Random_Seed(int seed)
{
	if (!seed)
	{
		g_seed=Timer_GetCurrentSystemTime();//timeGetTime();
		g_seed_self=g_seed;
	}
	else
	{
		g_seed=seed;
	}
	return g_seed;
}


int CALL HGEExport::Random_Int(int minval, int maxval, bool bSelf)
{
	unsigned int * usingseed = bSelf ? (&g_seed_self) : (&g_seed);
	*usingseed=214013*(*usingseed)+2531011;
	if (maxval-minval+1 == 0)
	{
		return minval;
	}
	return minval+((*usingseed) ^ (*usingseed)>>15)%(maxval-minval+1);
}


float CALL HGEExport::Random_Float(float minval, float maxval, bool bSelf)
{
	unsigned int * usingseed = bSelf ? (&g_seed_self) : (&g_seed);
	*usingseed=214013*(*usingseed)+2531011;
	//return min+g_seed*(1.0f/4294967295.0f)*(max-min);
	if (maxval == minval)
	{
		return minval;
	}
	return minval+(((*usingseed)>>16)*(maxval-minval))*(1.0f/65535.0f);
}

float CALL HGEExport::Timer_GetTime()
{
	return fTime;
}

float CALL HGEExport::Timer_GetDelta()
{
	return fDeltaTime;
}

float CALL HGEExport::Timer_GetFPS(int mod)
{
	if(mod < 1)
		return fFPS;
	else
	{
		static float nowFPS = 0;
		static float fLastTime = 0;
		static DWORD nLastFrameCounter = 0;
		int modbase = nFrameSkip < 2 ? nFrameCounter : (nFrameCounter / nFrameSkip);
		if((modbase) % mod == 1)
		{
			if (nFrameCounter > nLastFrameCounter && fTime > fLastTime)
			{
				nowFPS = 1 / ((fTime - fLastTime) / (nFrameCounter - nLastFrameCounter));
			}
			fLastTime = fTime;
			nLastFrameCounter = nFrameCounter;
		}
		return nowFPS;
	}
}

float CALL HGEExport::Timer_GetWorstFPS(int mod)
{
	if(mod <= 1)
		return fFPS;
	static float worstFPS = HGEFPS_MAX;
	static float worstFPSMod = worstFPS;
	if(worstFPS > fFPS)
		worstFPS = fFPS;
	int modbase = nFrameSkip < 2 ? nFrameCounter : (nFrameCounter / nFrameSkip);
	if((modbase) % mod == 1)
	{
		worstFPSMod = worstFPS;
		worstFPS = HGEFPS_MAX;
	}
	return worstFPSMod;
}

HEFFECT CALL HGEExport::Effect_Load(const char *filename, DWORD size/* =0 */)
{
	return NULL;
}

void CALL HGEExport::Effect_Free(HEFFECT eff)
{

}

HCHANNEL CALL HGEExport::Effect_Play(HEFFECT eff)
{
	return NULL;
}

HCHANNEL CALL HGEExport::Effect_PlayEx(HEFFECT eff, int volume/* =100 */, int pan/* =0 */, float pitch/* =1.0f */, bool loop/* =false */)
{
	return NULL;
}

HSTREAM CALL HGEExport::Stream_Load(const char *filename, DWORD size/* =0 */, bool bLoad/* =true */)
{
	return NULL;
}

void CALL HGEExport::Stream_Free(HSTREAM stream)
{
	
}

HCHANNEL CALL HGEExport::Stream_Play(HSTREAM stream, bool loop, int volume /* = 100 */)
{
	return NULL;
}

void	CALL HGEExport::	Channel_SetPanning(HCHANNEL chn, int pan)
{

}

void	CALL HGEExport::	Channel_SetVolume(HCHANNEL chn, int volume)
{

}

void	CALL HGEExport::	Channel_SetPitch(HCHANNEL chn, float pitch)
{

}

void	CALL HGEExport::	Channel_Pause(HCHANNEL chn)
{

}

void	CALL HGEExport::	Channel_Resume(HCHANNEL chn)
{

}

void	CALL HGEExport::	Channel_Stop(HCHANNEL chn)
{

}

void	CALL HGEExport::	Channel_PauseAll()
{

}

void	CALL HGEExport::	Channel_ResumeAll()
{

}

void	CALL HGEExport::	Channel_StopAll()
{

}

bool	CALL HGEExport::	Channel_IsPlaying(HCHANNEL chn)
{
	return false;
}

QWORD	CALL HGEExport::	Channel_GetLength(HCHANNEL chn)
{
	return 0;
}

QWORD	CALL HGEExport::	Channel_GetPos(HCHANNEL chn)
{
	return 0;
}

void	CALL HGEExport::	Channel_SetPos(HCHANNEL chn, QWORD pos)
{

}

void	CALL HGEExport::	Channel_SetStartPos(HCHANNEL chn, hgeChannelSyncInfo * pcsi)
{

}

void	CALL HGEExport::	Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan/* = -101*/, float pitch/* = -1*/)
{

}

bool	CALL HGEExport::	Channel_IsSliding(HCHANNEL channel)
{
	return false;
}

void	CALL HGEExport::	Channel_SetLoop(HCHANNEL channel, hgeChannelSyncInfo * pcsi)
{

}

void	CALL HGEExport::	Channel_RemoveLoop(HCHANNEL channel, hgeChannelSyncInfo * pcsi)
{

}

bool CALL HGEExport::Input_GetDIKey(int key, BYTE stateType /* = DIKEY_PRESSED */)
{
	return false;
}

bool CALL HGEExport::Input_SetDIKey(int key, bool set /* = true */)
{
	return true;
}

void	CALL HGEExport::		Gfx_Clear(DWORD color)
{

}

void	CALL HGEExport::		Gfx_RenderLine(float x1, float y1, float x2, float y2, DWORD color/* =0xFFFFFFFF */, float z/* =0 */)
{

}

void	CALL HGEExport::		Gfx_RenderTriple(const hgeTriple *triple)
{

}

void	CALL HGEExport::		Gfx_RenderQuad(const hgeQuad *quad)
{
	image_p texture = (image_p)quad->tex;
	RenderQuad(texture,
		quad->v[0].tx*texture->texw, quad->v[0].ty*texture->texh,
		(quad->v[1].tx-quad->v[0].tx)*texture->texw, (quad->v[3].ty-quad->v[0].ty)*texture->texh,
		quad->v[0].x, quad->v[0].y,
		1.0, 1.0,
		0, quad->v[0].col
		);
}

void	CALL HGEExport::		Gfx_SetClipping(int x/*=0*/, int y/*=0*/, int w/*=0*/, int h/*=0*/)
{

}

void	CALL HGEExport::		Gfx_SetTransform(float x/* =0 */, float y/* =0 */, float dx/* =0 */, float dy/* =0 */, float rot/* =0 */, float hscale/* =0 */, float vscale/* =0 */)
{

}

HTEXTURE	CALL HGEExport::Texture_Create(int width, int height)
{
	return NULL;
}

HTEXTURE	CALL HGEExport::Texture_Load(const char *filename, DWORD size/* =0 */, bool bMipmap/* =false */)
{
	void *data;
	DWORD _size;
	CTextureList *texItem;

	if(size) { data=(void *)filename; _size=size; }
	else
	{
		data=Resource_Load(filename, &_size);
		if(!data) return NULL;
	}
	image_p pTex = image_load_buf((const char*)data, _size, DISPLAY_PIXEL_FORMAT_8888, 1);

	if (!pTex)
	{	
		_PostError("Can't create texture");
		if(!size) Resource_Free(data);
		return NULL;
	}

	if(!size) Resource_Free(data);

	texItem=new CTextureList;
	texItem->tex=(HTEXTURE)pTex;
	texItem->width=pTex->texw;
	texItem->height=pTex->texh;
	texItem->next=textures;
	textures=texItem;

	return (HTEXTURE)pTex;
}

void		CALL HGEExport::Texture_Free(HTEXTURE tex)
{
	image_free((image_p)tex);
}

int			CALL HGEExport::Texture_GetWidth(HTEXTURE tex, bool bOriginal/* =false */)
{
	return 0;
}

int			CALL HGEExport::Texture_GetHeight(HTEXTURE tex, bool bOriginal/* =false */)
{
	return 0;
}

HD3DFONT    CALL HGEExport::Font_Load(const char * fontStyle,int height)
{
	return NULL;
}

void		CALL HGEExport::Font_Free(HD3DFONT font)
{

}

int			CALL HGEExport::Gfx_RenderText(HD3DFONT font, const char * text, float x, float y, float w, float h, DWORD color /* = 0xffffffff */)
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void HGEExport::_PostError(char * errorstr)
{
	System_Log(errorstr);
	strcpy(szError, errorstr);
}

LONGLONG HGEExport::Timer_GetCurrentSystemTime()
{
	return nge_get_tick_longlong();
}

void HGEExport::_SetFXVolume(int vol)
{

}
void HGEExport::_SetSampleVolume(int vol)
{

}
void HGEExport::_SetStreamVolume(int vol)
{

}