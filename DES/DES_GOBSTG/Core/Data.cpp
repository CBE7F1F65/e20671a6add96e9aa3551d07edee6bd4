#include "../Header/Data.h"
#include "../Header/BResource.h"
#include "../Header/Export.h"
#include "../../../src/hge/HGEExport.h"
#include "../Header/ConstResource.h"

#ifdef WIN32
#include <windows.h>
#endif

Data Data::data;

Data::Data()
{
	nowfilename = NULL;

	password = NULL;

	binmode = false;

	binfilename = NULL;
	binname = NULL;
//	spellaccessfilename = NULL;
//	rabinname = NULL;
//	scriptfilename = NULL;
//	scrbinname = NULL;
	resourcefilename = NULL;
	resbinname = NULL;

	customconstfilename = NULL;
//	spelldefinefilename = NULL;
	musicdefinefilename = NULL;
	bulletdefinefilename = NULL;
	enemydefinefilename = NULL;
	playerdefinefilename = NULL;
	spritedefinefilename = NULL;
	playershootdefinefilename = NULL;
	playerghostdefinefilename = NULL;

	datadefinefilename = NULL;
	packagedefinefilename = NULL;
	texturedefinefilename = NULL;
	effectdefinefilename = NULL;
	sedefinefilename = NULL;

	bin.clear();
}

Data::~Data()
{
}

void Data::GetIni()
{
	resourcefilename = Export::resourcefilename;
	resbinname = Export::resbinname;
	password = Export::GetPassword();
}

char * Data::translateSection(DWORD sec)
{
	strcpy(transbufs, "");
	for(int i=DATAMAPENUM_DATASTART; i<DATAMAPENUM_END; i++)
	{
		if((sec & ~DATASBINMASK_TYPE) == dataMap[i].binname)
		{
			strcat(transbufs, dataMap[i].textname);
			break;
		}
	}
	for(int i=DATAMAPENUM_DATASTART; i<DATAMAPENUM_END; i++)
	{
		if((sec & ~DATASBINMASK_DIFF) == dataMap[i].binname)
		{
			strcat(transbufs, dataMap[i].textname);
			break;
		}
	}
	for(int i=DATAMAPENUM_DATASTART; i<DATAMAPENUM_END; i++)
	{
		if((sec & ~DATASBINMASK_CHARA) == dataMap[i].binname)
		{
			strcat(transbufs, dataMap[i].textname);
			break;
		}
	}
	if(sec & ~DATASBINMASK_NUM)
	{
		char buffer[M_STRITOAMAX];
		strcat(transbufs, "_");
		sprintf(buffer, "%d", (sec & ~DATASBINMASK_NUM));
		strcat(transbufs, buffer);
	}
	return transbufs;
}

char * Data::translateName(DWORD name)
{
	strcpy(transbufn, "");
	for(int i=DATAMAPENUM_DATASTART; i<DATAMAPENUM_END; i++)
	{
		if((name & ~DATANBINMASK_TYPE) == dataMap[i].binname)
		{
			strcat(transbufn, dataMap[i].textname);
			break;
		}
	}
	if(name & ~DATASBINMASK_NUM)
	{
		char buffer[M_STRITOAMAX];
		strcat(transbufn, "_");
		sprintf(buffer, "%d", (name & ~DATASBINMASK_NUM));
		strcat(transbufn, buffer);
	}
	return transbufn;
}

DWORD Data::sLinkType( DWORD type)
{
	return dataMap[type].binname;
}

DWORD Data::sLinkNum(DWORD sec, DWORD num)
{
	sec &= DATASBINMASK_NUM;
	sec |= (num & ~DATASBINMASK_NUM);
	return sec;
}

DWORD Data::nLinkType(DWORD type)
{
	return dataMap[type].binname;
}

DWORD Data::nLinkNum(DWORD name, DWORD num)
{
	name &= DATASBINMASK_NUM;
	name |= (num & ~DATANBINMASK_NUM);
	return name;
}
/*
WORD Data::raGetIndi(int sno)
{
	return indi[sno];
}
void Data::raSetIndi(int sno, WORD _indi)
{
	indi[sno] = _indi;
}
*//*

int Data::getSpellNumber(int sno)
{
	return BResource::res.spelldata[sno].spellnumber;
}

int Data::getSpellUser(int sno)
{
	return BResource::res.spelldata[sno].userID;
}

char * Data::getSpellName(int sno)
{
	strcpy(buf, BResource::res.spelldata[sno].spellname);
	return buf;
}

char * Data::getSpellUserName(int sno)
{
	strcpy(buf, getEnemyName(BResource::res.spelldata[sno].userID));
	return buf;
}

char * Data::getSpellUserEName(int sno)
{
	strcpy(buf, getEnemyEName(BResource::res.spelldata[sno].userID));
	return buf;
}
*/

char * Data::getEnemyName(int type)
{
	if (type < 0 || type >= ENEMYTYPEMAX)
	{
		return NULL;
	}
	return BResource::pbres->enemydata[type].name;
}

char * Data::getEnemyEName(int type)
{
	if (type < 0 || type >= ENEMYTYPEMAX)
	{
		return NULL;
	}
	return BResource::pbres->enemydata[type].ename;
}

char * Data::getPlayerName(int type)
{
	if (type < 0 || type >= PLAYERTYPEMAX)
	{
		return NULL;
	}
	return BResource::pbres->playerdata[type].name;
}

char * Data::getPlayerEName(int type)
{
	if (type < 0 || type >= PLAYERTYPEMAX)
	{
		return NULL;
	}
	return BResource::pbres->playerdata[type].ename;
}

BYTE * Data::CreateMemHeader(BYTE type)
{
	BYTE * memheader = (BYTE *)malloc(M_BINHEADER_OFFSET);
	ZeroMemory(memheader, M_BINHEADER_OFFSET);

	char buffer[M_STRMAX];
	char tbuff[M_STRITOAMAX];
	strcpy(buffer, "[");
	strcat(buffer, translateSection(sLinkType(DATAS_HEADER)));
	strcat(buffer, "]\r\n");
	strcat(buffer, translateName(nLinkType(DATAN_GAMEVERSION)));
	strcat(buffer, "=");
	sprintf(tbuff, "%d", GAME_VERSION);
	strcat(buffer, tbuff);
	strcat(buffer, "\r\n");
	strcat(buffer, translateName(nLinkType(DATAN_SIGNATURE)));
	strcat(buffer, "=");
	strcat(buffer, GAME_SIGNATURE);
	strcat(buffer, "\r\n");
	strcat(buffer, translateName(nLinkType(DATAN_FILETYPE)));
	strcat(buffer, "=");
	sprintf(tbuff, "%d", type);
	strcat(buffer, tbuff);
	strcat(buffer, "\r\n");

	memcpy(memheader, buffer, strlen(buffer));

	return memheader;
}

bool Data::CheckMemHeader(const BYTE * memdata, DWORD size, BYTE type)
{
	bool ret = true;
	if(!memdata || size < M_BINHEADER_OFFSET)
		return false;

	BYTE * memheader = CreateMemHeader(type);
	if(strncmp((char *)memheader, (char *)memdata, M_BINHEADER_OFFSET))
	{
		ret = false;
	}
	free(memheader);

	return ret;
}

void Data::getFile(BYTE type)
{
	switch(type)
	{
	case DATA_RESOURCEFILE:
		nowfilename = resourcefilename;
		break;
/*
	case DATA_SCRIPTFILE:
		nowfilename = scriptfilename;
		break;
*/
	case DATA_CUSTOMCONSTFILE:
		nowfilename = customconstfilename;
		break;
		/*
	case DATA_SPELLACCESSFILE:
		nowfilename = spellaccessfilename;
		break;
		
	case DATA_SPELLDEFINEFILE:
		nowfilename = spelldefinefilename;
		break;
		*/

	case DATA_MUSICDEFINEFILE:
		nowfilename = musicdefinefilename;
		break;

	case DATA_BULLETDEFINEFILE:
		nowfilename = bulletdefinefilename;
		break;
	case DATA_ENEMYDEFINEFILE:
		nowfilename = enemydefinefilename;
		break;
	case DATA_PLAYERDEFINEFILE:
		nowfilename = playerdefinefilename;
		break;
	case DATA_SPRITEDEFINEFILE:
		nowfilename = spritedefinefilename;
		break;
	case DATA_PLAYERSHOOTDEFINE:
		nowfilename = playershootdefinefilename;
		break;
	case DATA_PLAYERGHOSTDEFINE:
		nowfilename = playerghostdefinefilename;
		break;

	case DATA_DATATABLEDEFINE:
		nowfilename = datadefinefilename;
		break;
	case DATA_PACKAGETABLEDEFINE:
		nowfilename = packagedefinefilename;
		break;
	case DATA_TEXTURETABLEDEFINE:
		nowfilename = texturedefinefilename;
		break;
	case DATA_EFFECTTABLEDEFINE:
		nowfilename = effectdefinefilename;
		break;
	case DATA_SETABLEDEFINE:
		nowfilename = sedefinefilename;
		break;

	case DATA_BINFILE:
		nowfilename = binfilename;
		break;

	default:
		nowfilename = NULL;
	}
}

bool Data::Init(BYTE type)
{
	if(binmode && (type & DATA_TABLEHEADER) ||
		(type == DATA_RESOURCEFILE))
	{
		return true;
	}

	getFile(type);
	if(!nowfilename)
		return false;

	io_fdelete(hge->Resource_MakePath(nowfilename));

	if(type == DATA_BINFILE)
		bin.clear();

	if((type & DATA_MEMORYHEADER) ||
		(type == DATA_RESOURCEFILE) && binmode)
	{
		char * fname;
		char * bname;

		switch(type)
		{
			/*
		case DATA_SCRIPTFILE:
			fname = scriptfilename;
			bname = scrbinname;
			break;
			*/

		case DATA_RESOURCEFILE:
			fname = resourcefilename;
			bname = resbinname;
			break;
/*
		case DATA_SPELLACCESSFILE:
			fname = spellaccessfilename;
			bname = rabinname;
			break;
			*/
		}

		BYTE * _content;
		DWORD _size;

		BYTE * _memheader;
		_memheader = CreateMemHeader(type);

		/*
		if(type == DATA_SPELLACCESSFILE)
		{
			_size = M_BINHEADER_OFFSET + sizeof(WORD) * DATARA_INDIMAX;
			_content = (BYTE *)malloc(_size);
			memcpy(_content, _memheader, M_BINHEADER_OFFSET);
			free(_memheader);
			BYTE * it = _content + M_BINHEADER_OFFSET;
			for(int i=0; i<DATARA_INDIMAX; i++)
			{
				*(WORD *)it = indi[i];
				it += DATARA_INDI_SIZE;
			}
		}
		else
		{
		*/
			_size = M_BINHEADER_OFFSET;
			_content = _memheader;
//		}

		hgeMemoryFile memfile;
		memfile.data = _content;
		memfile.filename = bname;
		memfile.size = _size;

		hge->Resource_CreatePack(fname, password, &memfile, NULL);
		hge->Resource_RemovePack(fname);

		free(_content);
	}
	
	if(type == DATA_BINFILE)
	{
		hgeMemoryFile memfile;
		memfile.filename = binname;
		memfile.data = NULL;
		memfile.size = 0;

		hge->Resource_CreatePack(nowfilename, password, &memfile, NULL);
		hge->Resource_RemovePack(nowfilename);

		iWrite(type, sLinkType(DATAS_HEADER), nLinkType(DATAN_GAMEVERSION), GAME_VERSION);
		sWrite(type, sLinkType(DATAS_HEADER), nLinkType(DATAN_SIGNATURE), GAME_SIGNATURE);
		iWrite(type, sLinkType(DATAS_HEADER), nLinkType(DATAN_FILETYPE), type);
	}
#ifdef __DEBUG
	HGELOG("Succeeded in rebuilding Data File %s.", nowfilename);
#endif
	return true;
}

bool Data::SetFile(const char * _filename, BYTE type)
{
	getFile(type);
	if(!nowfilename)
		return false;
	strcpy(nowfilename, hge->Resource_MakePath(_filename));

	if(type == DATA_BINFILE)
	{
		hge->Resource_AttachPack(nowfilename, password);
		BYTE * content = NULL;
		DWORD size;
		content = hge->Resource_Load(binname, &size);
		hge->Resource_RemovePack(nowfilename);
		if (content)
		{
			MemToList(content, size);
			hge->Resource_Free(content);
		}
	}
	if(/*_access(nowfilename, 00) == -1 ||*/
		!CheckHeader(type))
	{
		if(type == DATA_BINFILE || (type & DATA_MEMORYHEADER))
		{
			if(!Init(type))
			{
				goto failed;
			}
		}
		else
		{
failed:
#ifdef __DEBUG
				HGELOG("%s\nFailed in Getting Data File %s.", HGELOG_ERRSTR, nowfilename);
#endif
				strcpy(nowfilename, "");
				nowfilename = NULL;
				return false;
		}
	}
/*
	if(type == DATA_SPELLACCESSFILE)
	{
		Export::Resource_AttachPack(nowfilename, password);
		BYTE * content;
		DWORD size;
		content = Export::Resource_Load(rabinname, &size);
		BYTE * it = NULL;
		if(content)
			it = content + M_BINHEADER_OFFSET;
		for(int i=0; i<DATARA_INDIMAX; i++)
		{
			indi[i] = content ? *(WORD *)(it) : 0;
			if(content)
				it += DATARA_INDI_SIZE;
		}
		Export::Resource_Free(content);
	}
*/
	return true;
}

bool Data::GetAllTable()
{
	//data
	if (!GetTableFile(DATA_DATATABLEDEFINE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading DataDefineFile %s.", HGELOG_ERRSTR, datadefinefilename);
#endif // __DEBUG
		return false;
	}
	//package
	if (!GetTableFile(DATA_PACKAGETABLEDEFINE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading PackageDefineFile %s.", HGELOG_ERRSTR, packagedefinefilename);
#endif // __DEBUG
		return false;
	}
	//texture
	if (!GetTableFile(DATA_TEXTURETABLEDEFINE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading TextureDefineFile %s.", HGELOG_ERRSTR, texturedefinefilename);
#endif // __DEBUG
		return false;
	}
	//effect
	if (!GetTableFile(DATA_EFFECTTABLEDEFINE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading EffectDefineFile %s.", HGELOG_ERRSTR, effectdefinefilename);
#endif // __DEBUG
		return false;
	}
	//se
	if (!GetTableFile(DATA_SETABLEDEFINE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading SEDefineFile %s.", HGELOG_ERRSTR, sedefinefilename);
#endif // __DEBUG
		return false;
	}

	//music
	if (!GetTableFile(DATA_MUSICDEFINEFILE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading MusicDefineFile %s.", HGELOG_ERRSTR, musicdefinefilename);
#endif // __DEBUG
		return false;
	}

	//sprite
	if (!GetTableFile(DATA_SPRITEDEFINEFILE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading SpriteDefineFile %s.", HGELOG_ERRSTR, spritedefinefilename);
#endif // __DEBUG
		return false;
	}
/*
	if (!GetTableFile(DATA_SPELLDEFINEFILE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading SpellDefineFile %s.", HGELOG_ERRSTR, spelldefinefilename);
#endif // __DEBUG
		return false;
	}
*/
	if (!GetTableFile(DATA_BULLETDEFINEFILE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading BulletDefineFile %s.", HGELOG_ERRSTR, bulletdefinefilename);
#endif // __DEBUG
		return false;
	}

	//boss
	if (!GetTableFile(DATA_ENEMYDEFINEFILE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading EnemyDefineFile %s.", HGELOG_ERRSTR, enemydefinefilename);
#endif // __DEBUG
		return false;
	}

	//player
	if (!GetTableFile(DATA_PLAYERDEFINEFILE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading PlayerDefineFile %s.", HGELOG_ERRSTR, playerdefinefilename);
#endif // __DEBUG
		return false;
	}

	//playershoot
	if (!GetTableFile(DATA_PLAYERSHOOTDEFINE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading PlayerShootDefineFile %s.", HGELOG_ERRSTR, playershootdefinefilename);
#endif // __DEBUG
		return false;
	}

	//playerghost
	if (!GetTableFile(DATA_PLAYERGHOSTDEFINE))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in loading PlayerGhostDefineFile %s.", HGELOG_ERRSTR, playerghostdefinefilename);
#endif // __DEBUG
		return false;
	}

	return true;
}

bool Data::CheckHeader(BYTE type)
{
	if(/*type == DATA_SCRIPTFILE ||*/
		type == DATA_RESOURCEFILE && binmode/*
		 ||
				type == DATA_SPELLACCESSFILE*/
		)
	{
		char * fname;
		char * bname;
		switch(type)
		{
			/*
		case DATA_SCRIPTFILE:
			fname = scriptfilename;
			bname = scrbinname;
			break;
			*/
		case DATA_RESOURCEFILE:
			fname = resourcefilename;
			bname = resbinname;
			break;
			/*
		case DATA_SPELLACCESSFILE:
			fname = spellaccessfilename;
			bname = rabinname;
			break;
			*/
		}
		BYTE * _content = NULL;
		DWORD _size;
		bool tbret = hge->Resource_AttachPack(fname, password);
		_content = hge->Resource_Load(bname, &_size);
		hge->Resource_RemovePack(fname);
		bool bret = CheckMemHeader(_content, _size, type);
		hge->Resource_Free(_content);
		if(!bret)
		{
			return false;
		}
	}
	/*
	else
	{
		if(iRead(type, sLinkType(DATAS_HEADER), nLinkType(DATAN_GAMEVERSION), -1) != GAME_VERSION ||
			strcmp(sRead(type, sLinkType(DATAS_HEADER), nLinkType(DATAN_SIGNATURE), ""), GAME_SIGNATURE) ||
			iRead(type, sLinkType(DATAS_HEADER), nLinkType(DATAN_FILETYPE), -1) != type)
			return false;
	}
	*/
	return true;
}

bool Data::SaveBin()
{
	bool ret = true;
	//binfile
	getFile(DATA_BINFILE);
	BYTE * _bin;
	DWORD _size;
	_bin = ListToMem(&_size);

	hgeMemoryFile memfile;
	memfile.filename = binname;
	memfile.data = _bin;
	memfile.size = _size;

	if (!hge->Resource_CreatePack(nowfilename, password, &memfile, NULL))
	{
		ret = false;
	}
	hge->Resource_RemovePack(nowfilename);

	bin.clear();
	free(_bin);

	if (!ret)
	{
		return false;
	}

	//spellaccessfile
	/*
	getFile(DATA_SPELLACCESSFILE);
	BYTE * _radata;
	_size = DATARA_INDIMAX * DATARA_INDI_SIZE + DATA_HEADEROFFSET;
	_radata = (BYTE *)malloc(_size);

	BYTE * _raheader;
	_raheader = Export::Resource_Load(rabinname);
	if(_raheader)
	{
		memcpy(_radata, _raheader, DATA_HEADEROFFSET);
	}
	Export::Resource_Free(_raheader);

	BYTE * rait = _radata+DATA_HEADEROFFSET;

	for(int i=0; i<DATARA_INDIMAX; i++)
	{
		*(WORD *)rait = indi[i];
		rait += DATARA_INDI_SIZE;
	}

	memfile.filename = rabinname;
	memfile.data = _radata;
	memfile.size = _size;

	if (!Export::Resource_CreatePack(nowfilename, password, &memfile, NULL))
	{
		ret = false;
	}

	free(_radata);
*/
	return ret;
}

bool Data::MemToList(BYTE * memcontent, DWORD size)
{
	bin.clear();
	for(BYTE * it=memcontent; it<memcontent+size; it+=sizeof(dataBin))
	{
		bool bFound = false;
		dataBin _bin;
		memcpy(&_bin, it, sizeof(dataBin));
		for(vector<dataSection>::iterator i=bin.begin(); i!=bin.end(); i++)
		{
			if(i->section == _bin.section)
			{
				for(vector<dataName>::iterator j=i->data.begin(); j!=i->data.end(); j++)
				{
					if(j->name == _bin.name)
					{
						j->value = _bin.value;
						bFound = true;
						break;
					}
				}
				if(!bFound)
				{
					dataName _dataname;
					_dataname.name = _bin.name;
					_dataname.value = _bin.value;
					i->data.push_back(_dataname);
				}
				bFound = true;
				break;
			}
		}
		if(!bFound)
		{
			dataSection _datasec;
			dataName _dataname;
			_dataname.value = _bin.value;
			_dataname.name = _bin.name;
			_datasec.section = _bin.section;
			_datasec.data.push_back(_dataname);
			bin.push_back(_datasec);
		}
	}
	return true;
}

LONGLONG * Data::AttachValue(DWORD section, DWORD name, LONGLONG def_val)
{
	for(vector<dataSection>::iterator i=bin.begin(); i!=bin.end(); i++)
	{
		if(i->section == section)
		{
			for(vector<dataName>::iterator j=i->data.begin(); j!=i->data.end(); j++)
			{
				if(j->name == name)
					return &(j->value);
			}
			dataName _dataname;
			_dataname.name = name;
			_dataname.value = def_val;
			i->data.push_back(_dataname);
			return &(i->data.back().value);
		}
	}
	dataSection _datasec;
	dataName _dataname;
	_dataname.name = name;
	_dataname.value = def_val;
	_datasec.data.push_back(_dataname);
	_datasec.section = section;
	bin.push_back(_datasec);
	return &(bin.back().data.back().value);
}

BYTE * Data::ListToMem(DWORD * size)
{
	BYTE * memcontent;
	*size = 0;
	for(vector<dataSection>::iterator i=bin.begin(); i!=bin.end(); i++)
	{
		*size += (i->data.size()) * sizeof(dataBin);
	}
	memcontent = (BYTE *)malloc(*size);
	BYTE * it = memcontent;
	for(vector<dataSection>::iterator i=bin.begin(); i!=bin.end(); i++)
	{
		for(vector<dataName>::iterator j=i->data.begin(); j!=i->data.end(); j++)
		{
			dataBin _bin;
			_bin.section = i->section;
			_bin.name = j->name;
			_bin.value = j->value;
			memcpy(it, &_bin, sizeof(dataBin));
			it += sizeof(dataBin);
		}
	}
	return memcontent;
}

bool Data::iWrite(BYTE type, DWORD section, DWORD name, int value)
{
	getFile(type);
	if(nowfilename)
	{
		if(type == DATA_BINFILE)
		{
			*AttachValue(section, name) = value;
			return true;
		}
		else
		{
#ifdef WIN32
			sprintf(buf, "%d", value);
			if(WritePrivateProfileString(translateSection(section), translateName(name), buf, nowfilename))
				return true;
#else
			return true;
#endif
		}
	}
	return false;
}
int Data::iRead(BYTE type, DWORD section, DWORD name, int def_val)
{
	getFile(type);
	if(nowfilename)
	{
		if(type == DATA_BINFILE)
		{
			return (int)(*AttachValue(section, name, def_val));
		}
		else
		{
#ifdef WIN32
			if(GetPrivateProfileString(translateSection(section), translateName(name), "", buf, sizeof(buf), nowfilename))
			{
				return atoi(buf);
			}
#endif
		}
	}
	return def_val;
}
bool Data::lWrite(BYTE type, DWORD section, DWORD name, LONGLONG value)
{
	getFile(type);
	if(nowfilename)
	{
		if(type == DATA_BINFILE)
		{
			*AttachValue(section, name) = value;
			return true;
		}
		else
		{
#ifdef WIN32
			_i64toa(value, buf, 10);
			if(WritePrivateProfileString(translateSection(section), translateName(name), buf, nowfilename))
				return true;
#else
			return true;
#endif
		}
	}
	return false;
}
LONGLONG Data::lRead(BYTE type, DWORD section, DWORD name, LONGLONG def_val)
{
	getFile(type);
	if(nowfilename)
	{
		if(type == DATA_BINFILE)
		{
			return *AttachValue(section, name, def_val);
		}
		else
		{
#ifdef WIN32
			if(GetPrivateProfileString(translateSection(section), translateName(name), "", buf, sizeof(buf), nowfilename))
			{
				return _atoi64(buf);
			}
#endif
		}
	}
	return def_val;
}
bool Data::fWrite(BYTE type, DWORD section, DWORD name, float value)
{
	getFile(type);
	if(nowfilename)
	{
		if(type == DATA_BINFILE)
		{
			*AttachValue(section, name) = *((DWORD *)(&value));
			return true;
		}
		else
		{
#ifdef WIN32
			sprintf(buf, "%f", value);
			if(WritePrivateProfileString(translateSection(section), translateName(name), buf, nowfilename))
				return true;
#else
			return true;
#endif
		}
	}
	return false;
}
float Data::fRead(BYTE type, DWORD section, DWORD name, float def_val)
{
	getFile(type);
	if(nowfilename)
	{
		if(type == DATA_BINFILE)
		{
			return *((float *)(AttachValue(section, name, *((float *)&def_val))));
		}
		else
		{
#ifdef WIN32
			if(GetPrivateProfileString(translateSection(section), translateName(name), "", buf, sizeof(buf), nowfilename))
			{
				return float(atof(buf));
			}
#endif
		}
	}
	return def_val;
}
bool Data::sWrite(BYTE type, DWORD section, DWORD name, const char * value)
{
	getFile(type);
	if(nowfilename)
	{
		if(type == DATA_BINFILE)
		{
			*AttachValue(section, name) = *((LONGLONG *)value);
			return true;
		}
		else
		{
#ifdef WIN32
			if(WritePrivateProfileString(translateSection(section), translateName(name), value, nowfilename))
				return true;
#else
			return true;
#endif
		}
	}
	return false;
}
char * Data::sRead(BYTE type, DWORD section, DWORD name, const char * def_val)
{
	getFile(type);
	if(nowfilename)
	{
		if(type == DATA_BINFILE)
		{
			return (char *)(AttachValue(section, name, *((LONGLONG *)def_val)));
		}
		else
		{
#ifdef WIN32
			GetPrivateProfileString(translateSection(section), translateName(name), def_val, buf, sizeof(buf), nowfilename);
#endif
		}
	}
	else
		strcpy(buf, def_val);
	return buf;
}

//
bool Data::SetEffectSystemResourceName(int effi, const char * filename)
{
	if(effi < 0 || effi >= EFFECTSYSTYPEMAX)
		return false;
	strcpy(BResource::pbres->resdata.effectsysfilename[effi], filename);
	FILE * file = checkTableFile(DATA_EFFECTTABLEDEFINE);
	if (file == NULL)
	{
		return false;
	}
	char comment[2][M_STRMAX];
	fscanf(file, "%s%s", comment[0], comment[1]);
	fclose(file);
	file = NULL;

	getFile(DATA_EFFECTTABLEDEFINE);
	file = fopen(nowfilename, "wb");
	if (!file)
	{
		return false;
	}
	fprintf(file, "%x\t%s\t%x\r\n", GAME_VERSION, GAME_SIGNATURE, DATA_EFFECTTABLEDEFINE);
	fprintf(file, "%s\t%s\r\n", comment[0], comment[1]);
	for (int i=0; i<EFFECTSYSTYPEMAX; i++)
	{
		if (strlen(BResource::pbres->resdata.effectsysfilename[i]))
		{
			fprintf(file, "%d\t%s\r\n", i, BResource::pbres->resdata.effectsysfilename[i]);
		}
	}
	fclose(file);

	/*
	DWORD sec = sLinkType(RESDATAS_EFFECTSYS);
	DWORD name = nLinkNum(nLinkType(RESDATAN_TYPE), effi+1);
	if(!filename)
	{
		sWrite(DATA_RESOURCEFILE, sec, name, "");
	}
	else
	{
		sWrite(DATA_RESOURCEFILE, sec, name, filename);
	}
	*/
	return true;
}

bool Data::GetEffectSystemResourceName(int effi, char * filename)
{
	if(!filename || effi < 0 || effi >= EFFECTSYSTYPEMAX)
		return false;
	if (!strlen(BResource::pbres->resdata.effectsysfilename[effi]))
	{
		return false;
	}
	strcpy(filename, BResource::pbres->resdata.effectsysfilename[effi]);
	/*
	DWORD sec = sLinkType(RESDATAS_EFFECTSYS);
	DWORD name = nLinkNum(nLinkType(RESDATAN_TYPE), effi+1);
	char buffer[M_STRMAX];
	strcpy(buffer, sRead(DATA_RESOURCEFILE, sec, name, ""));
	if(!strlen(buffer))
		return false;
	strcpy(filename, buffer);
	*/
	return true;
}
/*

int Data::nMeet(int sno, bool bSpell / * = false * /)
{
	DWORD sec;
	sec = sLinkType(DATAS_SPELL);
	sec = sLinkNum(sec, sno);
	if(bSpell)
	{
		return iRead(DATA_BINFILE, sec, nLinkType(DATAN_MEETPRACTICE), 0);
	}
	else
	{
		return iRead(DATA_BINFILE, sec, nLinkType(DATAN_MEETGAME), 0);
	}
}

int Data::nGet(int sno, bool bSpell / * = false * /)
{
	DWORD sec;
	sec = sLinkType(DATAS_SPELL);
	sec = sLinkNum(sec, sno);
	if(bSpell)
	{
		return iRead(DATA_BINFILE, sec, nLinkType(DATAN_GETPRACTICE), 0);
	}
	else
	{
		return iRead(DATA_BINFILE, sec, nLinkType(DATAN_GETGAME), 0);
	}
}

LONGLONG Data::nHighScore(int sno, BYTE difflv, bool bSpell / * = false * /, bool bPractice / * = false * /)
{
	DWORD sec;
	if(bSpell)
	{
		sec = sLinkType(DATAS_SPELL);
		sec = sLinkNum(sec, sno);
		return lRead(DATA_BINFILE, sec, nLinkType(DATAN_TOPBONUS), 0);
	}

	if(bPractice)
	{
		sec = sLinkType(DATAS_STAGEPRACTICE);
		sec = sLinkDiff(sec, difflv);
		sec = sLinkNum(sec, sno / M_STAGENSCENE);
		return lRead(DATA_BINFILE, sec, nLinkType(DATAN_TOPSCORE), 0);
	}

	sec = sLinkType(DATAS_TOP);
	sec = sLinkDiff(sec, difflv);
	sec = sLinkNum(sec, 1);

	return lRead(DATA_BINFILE, sec, nLinkType(DATAN_SCORE), 0);
}

int Data::nTryStageTime(int stage, BYTE difflv)
{
	DWORD sec;

	sec = sLinkType(DATAS_STAGEPRACTICE);
	sec = sLinkDiff(sec, difflv);
	sec = sLinkNum(sec, stage);

	return iRead(DATA_BINFILE, sec, nLinkType(DATAN_TRYTIME), 0);
}*/

