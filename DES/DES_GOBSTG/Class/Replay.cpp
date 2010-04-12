#include "../header/Replay.h"
#include "../header/Process.h"
#include "../header/Player.h"
#include "../header/Data.h"
#include "../header/BResource.h"
#include "../header/Player.h"
#include "../header/SE.h"
#include "../header/ConstResource.h"

list<_ReplayNameListItem> Replay::_rpyfilenamelist;

Replay Replay::rpy;
Replay Replay::enumrpy[RPYENUMMAX];
int Replay::nenumrpy = 0;

Replay::Replay()
{
	replayIndex = 0;
	ZeroMemory(&replayframe, sizeof(replayFrame) * M_SAVEINPUTMAX);
	ZeroMemory(&rpyinfo, sizeof(replayInfo));
	strcpy(filename, "");
}

Replay::~Replay()
{
}

void Replay::Release(bool deletefiles/* =true */)
{
	for (list<_ReplayNameListItem>::iterator it=_rpyfilenamelist.begin(); it!=_rpyfilenamelist.end();)
	{
		if (deletefiles)
		{
			DeleteFile(it->filename);
		}
		it = _rpyfilenamelist.erase(it);
	}
}

void Replay::Free(const char * filename)
{
	if (filename)
	{
		Export::rpyFree(filename);
	}
}

int Replay::GetEnumReplay()
{
	ReleaseEnumReplay();

	SetCurrentDirectory(hge->Resource_MakePath(BResource::res.resdata.replayfoldername));
	char * buffer;
	char enumfile[M_STRMAX];
	strcpy(enumfile, BResource::res.resdata.replayfoldername);
	strcat(enumfile, "*.");
	strcat(enumfile, BResource::res.resdata.replayextensionname7);
	buffer = hge->Resource_EnumFiles(enumfile);

	int tnrpys = 0;
	while(buffer != NULL && tnrpys < RPYENUMMAX)
	{
		if(enumrpy[tnrpys].Load(buffer))
		{
			tnrpys++;
		}
		if(!hge->Resource_EnumFiles())
		{
			break;
		}
	}
	nenumrpy = tnrpys;
	return nenumrpy;
}

void Replay::ReleaseEnumReplay()
{
	for (int i=0; i<RPYENUMMAX; i++)
	{
		if (strlen(enumrpy[i].filename))
		{
			Export::rpyFree(enumrpy[i].filename);
			strcpy(enumrpy[i].filename, "");
		}
	}
	nenumrpy = 0;
}

void Replay::Fill()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

//	rpyinfo.modeflag = (Process::mp.spellmode?M_RPYMODE_SPELL:0)|(Process::mp.practicemode?M_RPYMODE_PRACTICE:0);

	for (int i=0; i<M_PL_MATCHMAXPLAYER; i++)
	{
		rpyinfo.usingchara[i][0] = Player::p[i].ID;
		rpyinfo.usingchara[i][1] = Player::p[i].ID_sub_1;
		rpyinfo.usingchara[i][2] = Player::p[i].ID_sub_2;
		rpyinfo.initlife[i] = Player::p[i].initlife;
	}

	rpyinfo.scene = Process::mp.scene;
	rpyinfo.alltime = Process::mp.alltime;
	rpyinfo.year = systime.wYear;
	rpyinfo.month = systime.wMonth;
	rpyinfo.day = systime.wDay;
	rpyinfo.hour = systime.wHour;
	rpyinfo.minute = systime.wMinute;

	rpyinfo.lost = Player::lostStack / Process::mp.framecounter;
	rpyinfo.matchmode = Process::mp.matchmode;
	rpyinfo.offset = replayIndex;

	for (int i=0; i<M_PL_MATCHMAXPLAYER; i++)
	{
		// TODO:
		strcpy(rpyinfo.username[i], Process::mp.username[i]);
	}
	if (rpyinfo.matchmode == M_MATCHMODE_C2P || rpyinfo.matchmode == M_MATCHMODE_C2C)
	{
		strcpy(rpyinfo.username[0], RESCONFIGDEFAULT_USERNAME);
	}
	else if (rpyinfo.matchmode == M_MATCHMODE_P2C || rpyinfo.matchmode == M_MATCHMODE_C2C)
	{
		strcpy(rpyinfo.username[1], RESCONFIGDEFAULT_USERNAME);
	}

}

void Replay::partFill(BYTE part)
{
	if (part < RPYPARTMAX)
	{
		partinfo[part].offset = replayIndex + 1;
		partinfo[part].seed = Process::mp.seed;
	}
	else
		part = 0;
	for (int i=0; i<M_PL_MATCHMAXPLAYER; i++)
	{
		partinfo[part].nowID[i] = Player::p[i].nowID;
	}
}

bool Replay::Check(const char * _filename)
{
	if (!strlen(_filename))
	{
		return false;
	}
	BYTE * content;
	bool bret = false;

	strcpy(filename, _filename);
	char treplayfilename[M_PATHMAX];
	strcpy(treplayfilename, BResource::res.resdata.replayfoldername);
	strcat(treplayfilename, _filename);
	hge->Resource_AttachPack(treplayfilename, Data::data.password ^ REPLAYPASSWORD_XORMAGICNUM);

	content = hge->Resource_Load(hge->Resource_GetPackFirstFileName(treplayfilename));
	if(content)
	{
		if(strcmp((char *)(content + RPYOFFSET_SIGNATURE), BResource::res.resdata.replaysignature11))
			goto exit;
		if(*(DWORD *)(content + RPYOFFSET_VERSION) != GAME_VERSION)
			goto exit;
		if(strcmp((char *)(content + RPYOFFSET_COMPLETESIGN), BResource::res.resdata.replaycompletesign3))
			goto exit;
		bret = true;
	}
exit:
	hge->Resource_Free(content);
	if (!bret)
	{
		strcpy(filename, "");
	}
	return bret;
}

void Replay::WriteInput(WORD nowinput)
{
	replayIndex++;
	replayframe[replayIndex].input = nowinput;
	Export::rpySetBias(&(replayframe[replayIndex]));
}

WORD Replay::ReadInput()
{
	replayIndex++;
	return replayframe[replayIndex].input;
}

float Replay::GetReplayFPS()
{
	return Export::rpyGetReplayFPS(replayframe[replayIndex]);
}

void Replay::InitReplayIndex(bool replaymode, BYTE part)
{
	if (replaymode)
	{
		replayIndex = rpy.partinfo[part].offset - 1;
	}
	else
	{
		replayIndex = 0;
	}
}

bool Replay::Load(const char * _filename, bool getInput)
{
	bool ret = false;
	if(Check(_filename))
	{
        char treplayfilename[M_PATHMAX];
		strcpy(treplayfilename, BResource::res.resdata.replayfoldername);
		strcat(treplayfilename, _filename);
		ret = Export::rpyLoad(treplayfilename, &rpyinfo, partinfo, getInput ? replayframe : NULL);
		if (getInput)
		{
			replayIndex = 0;
		}
	}
	return ret;
}

void Replay::CreateSaveFilename(char * filename)
{
	// TODO:
	GetEnumReplay();
	char buffer[M_PATHMAX];
	sprintf(buffer, "%s_%02d%02d%02d_", RESDEFAULT_RPYSIGNATURE11, rpyinfo.year%100, rpyinfo.month, rpyinfo.day);
	char _filename[M_PATHMAX];
	for (int i=0; i<10000; i++)
	{
		sprintf(_filename, "%s%04d_", buffer, i);
		bool canuse = true;
		for (int j=0; j<RPYENUMMAX; j++)
		{
			if (!strncmp(_filename, enumrpy[j].filename, strlen(_filename)))
			{
				canuse = false;
				break;
			}
		}
		if (canuse)
		{
			buffer[0] = randt('a', 'z');
			buffer[1] = randt('0', '9');
			buffer[2] = randt('a', 'z');
			buffer[3] = randt('a', 'z');
			buffer[4] = 0;
			if (!strcmp(buffer, GAME_SIGNATURE))
			{
				SE::push(SE_ITEM_EXTEND);
			}
			strcat(_filename, buffer);
			strcat(_filename, ".");
			strcat(_filename, BResource::res.resdata.replayextensionname7);
			break;;
		}
	}
	strcpy(filename, _filename);
	ReleaseEnumReplay();
}

void Replay::Save(const char * replayfilename)
{
	char savefilename[M_PATHMAX];
	if(!replayfilename)
	{
		CreateSaveFilename(savefilename);
	}
	else
	{
		strcpy(savefilename, replayfilename);
	}

	strcpy(filename, savefilename);
	WriteInput(0xffff);

	char buffer[M_STRITOAMAX];

	DWORD _size = RPYOFFSET_INPUTDATA + (replayIndex + 1) * RPYSIZE_FRAME;
	BYTE * _rpydata = (BYTE *)malloc(_size);
	DWORD tdw;
	memcpy(_rpydata + RPYOFFSET_SIGNATURE, BResource::res.resdata.replaysignature11, RPYSIZE_SIGNATURE);
	tdw = GAME_VERSION;
	memcpy(_rpydata + RPYOFFSET_VERSION, &tdw, RPYSIZE_VERSION);
	memcpy(_rpydata + RPYOFFSET_COMPLETESIGN, BResource::res.resdata.replaycompletesign3, RPYSIZE_COMPLETESIGN);
	memcpy(_rpydata + RPYOFFSET_TAG, BResource::res.resdata.replaytag3, RPYSIZE_TAG);
	tdw = RPYOFFSET_PARTINFO;
	memcpy(_rpydata + RPYOFFSET_INFOOFFSET, &tdw, RPYSIZE_INFOOFFSET);
	strcpy(buffer, "");
	memcpy(_rpydata + RPYOFFSET_APPEND, buffer, RPYSIZE_APPEND);
	memcpy(_rpydata + RPYOFFSET_RPYINFO, &rpyinfo, RPYSIZE_RPYINFO);
	memcpy(_rpydata + RPYOFFSET_PARTINFO, partinfo, RPYSIZE_PARTINFO * RPYPARTMAX);
	memcpy(_rpydata + RPYOFFSET_INPUTDATA, replayframe, (replayIndex+1) * RPYSIZE_FRAME);

	/*
	replayFrame buff;
	buff.bias = 0;
	buff.input = 0xffff;
	memcpy(_rpydata + _size-sizeof(replayFrame), &buff, sizeof(replayFrame));
	*/

	char treplayfilename[M_PATHMAX];
	strcpy(treplayfilename, BResource::res.resdata.replayfoldername);
	strcat(treplayfilename, savefilename);

	char crcfilename[M_PATHMAX];
	strcpy(crcfilename, savefilename);
	strcat(crcfilename, itoa(hge->Resource_GetCRC(_rpydata, _size), buffer, 10));
	hgeMemoryFile memfile;
	memfile.filename = crcfilename;
	memfile.data = _rpydata;
	memfile.size = _size;

	if (hge->Resource_CreatePack(treplayfilename, Data::data.password ^ REPLAYPASSWORD_XORMAGICNUM, &memfile, NULL))
	{
		_ReplayNameListItem _item;
		strcpy(_item.filename, treplayfilename);
		_rpyfilenamelist.push_back(_item);
	}

	free(_rpydata);
}