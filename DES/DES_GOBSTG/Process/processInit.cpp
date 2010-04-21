#include "../Header/processPrep.h"

int Process::processPreInitial()
{
	BResource::Init();
	Replay::Init();

	bool rebuilddone = false;
	if(
#ifdef WIN32
		_access(CONFIG_STR_FILENAME, 00) == -1
#else
		true
#endif
		)
	{
rebuild:
		if (rebuilddone)
		{
			errorcode = PROC_ERROR_INIFILE;
			return PQUIT;
		}
		io_fdelete(hge->Resource_MakePath(CONFIG_STR_FILENAME));
//		DeleteFile(CONFIG_STR_FILENAME);

		hge->Ini_SetInt(Data::data.translateSection(Data::data.sLinkType(DATAS_HEADER)), Data::data.translateName(Data::data.nLinkType(DATAN_GAMEVERSION)), GAME_VERSION);
		hge->Ini_SetString(Data::data.translateSection(Data::data.sLinkType(DATAS_HEADER)), Data::data.translateName(Data::data.nLinkType(DATAN_SIGNATURE)), GAME_SIGNATURE);

		hge->Ini_SetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESOURCEFILE, RESCONFIGDEFAULT_RESOURCEFILE);
		hge->Ini_SetString(RESCONFIGS_RESOURCE, RESCONFIGN_RESBINNAME, RESCONFIGDEFAULT_RESBINNAME);
		hge->Ini_SetString(RESCONFIGS_RESOURCE, RESCONFIGN_PASSWORD, RESCONFIGDEFAULT_PASSWORD);

		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYUP, RESCONFIGDEFAULT_KEYUP_1);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYDOWN, RESCONFIGDEFAULT_KEYDOWN_1);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYLEFT, RESCONFIGDEFAULT_KEYLEFT_1);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYRIGHT, RESCONFIGDEFAULT_KEYRIGHT_1);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYFIRE, RESCONFIGDEFAULT_KEYFIRE_1);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYQUICK, RESCONFIGDEFAULT_KEYQUICK_1);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYSLOW, RESCONFIGDEFAULT_KEYSLOW_1);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYDRAIN, RESCONFIGDEFAULT_KEYDRAIN_1);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYPAUSE, RESCONFIGDEFAULT_KEYPAUSE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYSKIP, RESCONFIGDEFAULT_KEYSKIP);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYENTER, RESCONFIGDEFAULT_KEYENTER);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYESCAPE, RESCONFIGDEFAULT_KEYESCAPE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYCAPTURE, RESCONFIGDEFAULT_KEYCAPTURE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_KEYCOMBINESLOWDRAIN, RESCONFIGDEFAULT_KEYCOMBINESLOWDRAIN);

		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_JOYFIRE, RESCONFIGDEFAULT_JOYFIRE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_JOYQUICK, RESCONFIGDEFAULT_JOYQUICK);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_JOYSLOW, RESCONFIGDEFAULT_JOYSLOW);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_JOYDRAIN, RESCONFIGDEFAULT_JOYDRAIN);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_JOYPAUSE, RESCONFIGDEFAULT_JOYPAUSE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_JOYCOMBINESLOWDRAIN, RESCONFIGDEFAULT_JOYCOMBINESLOWDRAIN);

		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYUP, RESCONFIGDEFAULT_KEYUP_2);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYDOWN, RESCONFIGDEFAULT_KEYDOWN_2);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYLEFT, RESCONFIGDEFAULT_KEYLEFT_2);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYRIGHT, RESCONFIGDEFAULT_KEYRIGHT_2);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYFIRE, RESCONFIGDEFAULT_KEYFIRE_2);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYQUICK, RESCONFIGDEFAULT_KEYQUICK_2);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYSLOW, RESCONFIGDEFAULT_KEYSLOW_2);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYDRAIN, RESCONFIGDEFAULT_KEYDRAIN_2);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYPAUSE, RESCONFIGDEFAULT_KEYPAUSE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYSKIP, RESCONFIGDEFAULT_KEYSKIP);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYENTER, RESCONFIGDEFAULT_KEYENTER);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYESCAPE, RESCONFIGDEFAULT_KEYESCAPE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYCAPTURE, RESCONFIGDEFAULT_KEYCAPTURE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_KEYCOMBINESLOWDRAIN, RESCONFIGDEFAULT_KEYCOMBINESLOWDRAIN);

		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_JOYFIRE, RESCONFIGDEFAULT_JOYFIRE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_JOYQUICK, RESCONFIGDEFAULT_JOYQUICK);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_JOYSLOW, RESCONFIGDEFAULT_JOYSLOW);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_JOYDRAIN, RESCONFIGDEFAULT_JOYDRAIN);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_JOYPAUSE, RESCONFIGDEFAULT_JOYPAUSE);
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_2, RESCONFIGN_JOYCOMBINESLOWDRAIN, RESCONFIGDEFAULT_JOYCOMBINESLOWDRAIN);

		hge->Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLMUSIC, RESCONFIGDEFAULT_VOLMUSIC);
		hge->Ini_SetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLSE, RESCONFIGDEFAULT_VOLSE);

		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, RESCONFIGDEFAULT_SCREENMODE);
		hge->Ini_SetString(RESCONFIGS_CUSTOM, RESCONFIGN_USERNAME, RESCONFIGDEFAULT_USERNAME);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_RENDERSKIP, RESCONFIGDEFAULT_RENDERSKIP);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_1_1, RESCONFIGDEFAULT_LASTMATCHCHARA_1_1);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_1_2, RESCONFIGDEFAULT_LASTMATCHCHARA_1_2);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_1_3, RESCONFIGDEFAULT_LASTMATCHCHARA_1_3);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_2_1, RESCONFIGDEFAULT_LASTMATCHCHARA_2_1);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_2_2, RESCONFIGDEFAULT_LASTMATCHCHARA_2_2);
		hge->Ini_SetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_2_3, RESCONFIGDEFAULT_LASTMATCHCHARA_2_3);

		hge->Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_DEFAULTWINDOW, RESCONFIGDEFAULT_DEFAULTWINDOW);
		hge->Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWLEFT, RESCONFIGDEFAULT_WINDOWLEFT);
		hge->Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOP, RESCONFIGDEFAULT_WINDOWTOP);
		hge->Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWWIDTH, RESCONFIGDEFAULT_WINDOWWIDTH);
		hge->Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWHEIGHT, RESCONFIGDEFAULT_WINDOWHEIGHT);
		hge->Ini_SetInt(RESCONFIGS_CUSTOMWINDOW, RESCONFIGN_WINDOWTOPMOST, RESCONFIGDEFAULT_WINDOWTOPMOST);
#ifdef __DEBUG
		hge->Ini_SetInt(RESCONFIGS_KEYSETTING_1, RESCONFIGN_DEBUG_JOYSPEEDUP, RESCONFIGDEFAULT_DEBUG_JOYSPEEDUP);
		HGELOG("Succeeded in rebuilding Config File.");

		rebuilddone = true;
#endif
	}

	if (!GameInput::InitInput())
	{
		goto rebuild;
	}

	bgmvol			= hge->Ini_GetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLMUSIC, RESCONFIGDEFAULT_VOLMUSIC);
	sevol			= hge->Ini_GetInt(RESCONFIGS_VOLUME, RESCONFIGN_VOLSE, RESCONFIGDEFAULT_VOLSE);

	screenmode		= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_SCREENMODE, RESCONFIGDEFAULT_SCREENMODE);
	strcpy(username[0], hge->Ini_GetString(RESCONFIGS_CUSTOM, RESCONFIGN_USERNAME, RESCONFIGDEFAULT_USERNAME));
	strcpy(username[1], username[0]);
	renderskip		= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_RENDERSKIP, RESCONFIGDEFAULT_RENDERSKIP);

	lastmatchchara[0][0]	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_1_1, RESCONFIGDEFAULT_LASTMATCHCHARA_1_1);
	lastmatchchara[0][1]	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_1_2, RESCONFIGDEFAULT_LASTMATCHCHARA_1_2);
	lastmatchchara[0][2]	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_1_3, RESCONFIGDEFAULT_LASTMATCHCHARA_1_3);
	lastmatchchara[1][0]	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_2_1, RESCONFIGDEFAULT_LASTMATCHCHARA_2_1);
	lastmatchchara[1][1]	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_2_2, RESCONFIGDEFAULT_LASTMATCHCHARA_2_2);
	lastmatchchara[1][2]	= hge->Ini_GetInt(RESCONFIGS_CUSTOM, RESCONFIGN_LASTMATCHCHARA_2_3, RESCONFIGDEFAULT_LASTMATCHCHARA_2_3);

	//

	if(hge->Ini_GetInt(Data::data.translateSection(Data::data.sLinkType(DATAS_HEADER)), Data::data.translateName(Data::data.nLinkType(DATAN_GAMEVERSION)), -1) != GAME_VERSION)
		goto rebuild;
	
	if(screenmode < 0 || screenmode > 1)
		goto rebuild;
	if(bgmvol < 0 || bgmvol > 100)
		goto rebuild;
	if(sevol < 0 || sevol > 100)
		goto rebuild;
	if (renderskip < 0 || renderskip > 3)
	{
		goto rebuild;
	}
	else
	{
		if (renderskip == 0)
		{
			renderskip = 1;
		}
		hge->System_SetState(HGE_RENDERSKIP, renderskip * M_DEFAULT_RENDERSKIP);
	}

#ifdef __DEBUG
	HGELOG("Succeeded in gaining data from Config File.");
#endif
	hge->Resource_AttachPack(RESLOADING_PCK, Export::GetPassword());
	texInit = hge->Texture_Load(RESLOADING_TEX);

	return PGO;
}

int Process::processInit()
{
	gametime++;

	if (gametime == 1)
	{
		int iret = processPreInitial();
		return iret;
	}

	if (texInit)
	{
		hge->Texture_Free(texInit);
		texInit = NULL;
	}
	hge->Resource_RemovePack(RESLOADING_PCK);

	bool binmode = Export::GetResourceFile();

	Data::data.GetIni();

	if(binmode)
	{
		Data::data.binmode = true;
		Scripter::scr.binmode = true;
#ifdef __RELEASE
//		Export::System_SetState(HGE_LOGFILE, "");
#endif // __RELEASE
	}
	else
	{
		if(!BResource::pbres->Fill())
		{
#ifdef __DEBUG
			HGELOG("Error in Filling Resource Data.");
#endif
			errorcode = PROC_ERROR_RESOURCE;
			return PQUIT;
		}
		if(!Scripter::scr.LoadAll(tex))
		{
			errorcode = PROC_ERROR_SCRIPT;
			return PQUIT;
		}
		if(!BResource::pbres->Pack(Scripter::strdesc, BResource::pbres->customconstdata))
		{
#ifdef __DEBUG
			HGELOG("Error in Packing Resource Data.");
#endif
			errorcode = PROC_ERROR_TRANSLATE;
			return PQUIT;
		}
		if(!BResource::pbres->SetDataFile())
		{
			errorcode = PROC_ERROR_DATA;
			return PQUIT;
		}
	}
	if (binmode)
	{
		BResource::pbres->MallocCustomConst();
	}
	if(!BResource::pbres->Gain(Scripter::strdesc, /*binmode?*/BResource::pbres->customconstdata/*:NULL*/))
	{
#ifdef __DEBUG
		HGELOG("Error in Gaining Resource Data.");
#endif
		errorcode = PROC_ERROR_DATA;
		return PQUIT;
	}
	if(Scripter::scr.binmode && !Scripter::scr.LoadAll(tex))
	{
		errorcode = PROC_ERROR_SCRIPT;
		return PQUIT;
	}
	if(!Data::data.SetFile(Export::resourcefilename, DATA_RESOURCEFILE))
	{
#ifdef __DEBUG
		HGELOG("Error in Setting Resource File");
#endif
		errorcode = PROC_ERROR_DATA;
		return PQUIT;
	}

	for (int i=0; i<PACKAGEMAX; i++)
	{
		if(!BResource::pbres->LoadPackage(i))
		{
			errorcode = PROC_ERROR_PACKAGE;
			return PQUIT;
		}
	}

	BGLayer::Init(tex);

	SE::vol = sevol;
	if(!SE::Initial())
	{
		errorcode = PROC_ERROR_SOUND;
		return PQUIT;
	}

	tex[TEX_WHITE] = hge->Texture_Load(BResource::pbres->resdata.texfilename[TEX_WHITE]);
#ifdef __DEBUG
	if(tex[TEX_WHITE] == NULL)
	{
		HGELOG("%s\nFailed in loading Texture File %s.(To be assigned to Index %d).", HGELOG_ERRSTR, BResource::pbres->resdata.texfilename[TEX_WHITE], TEX_WHITE);
		errorcode = PROC_ERROR_TEXTURE;
		return PQUIT;
	}
	else
	{
		HGELOG("Succeeded in loading Texture File %s.(Assigned to Index %d).", BResource::pbres->resdata.texfilename[TEX_WHITE], TEX_WHITE);
	}
#endif

	char tnbuffer[M_STRMAX];
	for(int i=1;i<TEXMAX;i++)
	{
		//
		if(tex[i])
			hge->Texture_Free(tex[i]);
		tex[i] = NULL;

		strcpy(tnbuffer, BResource::pbres->resdata.texfilename[i]);
		if(strlen(tnbuffer))
		{
			tex[i] = hge->Texture_Load(tnbuffer);
//			strcpy(tnbuffer, BResource::res.resdata.texfilename[TEX_WHITE]);
		}

		if(tex[i] == NULL)
		{
#ifdef __DEBUG
			HGELOG("%s\nFailed in loading Texture File %s.(To be assigned to Index %d).", HGELOG_ERRSTR, tnbuffer, i);
			errorcode = PROC_ERROR_TEXTURE;
#endif
//			tex[i] = tex[TEX_WHITE];
			tex[i] = hge->Texture_Create(0, 0);
//			return PQUIT;
		}
#ifdef __DEBUG
		else
		{
			HGELOG("Succeeded in loading Texture File %s.(Assigned to Index %d).", tnbuffer, i);
		}
#endif
	}

	SpriteItemManager::Init(tex);

	Fontsys::Init();
	if(!Effectsys::Init(tex, BResource::pbres->resdata.effectsysfoldername, BResource::pbres->resdata.effectsysfilename))
	{
#ifdef __DEBUG
		HGELOG("%s\nFailed in Initializing Effectsys.", HGELOG_ERRSTR);
#endif
		return false;
	}

#ifdef __DEBUG
	HGELOG("Succeeded in loading all resources.\n");
#endif

	hge->System_SetState(HGE_WINDOWED, !(bool)screenmode);
	hge->System_SetState(HGE_HIDEMOUSE, (bool)screenmode);

	if(!screenmode)
	{
		Export::clientAdjustWindow();
	}

	Chat::chatitem.Init();

	SelectSystem::ClearAll();

	errorcode = PROC_ERROR_NONE;
	titleselect = 0;

	replaymode = false;

	playing = false;
	playtimeStart = 0;

	gametime = 0;
	state = STATE_TITLE;
	return PTURN;
}