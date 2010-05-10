#ifndef _MAINDEPENDENCY_H
#define _MAINDEPENDENCY_H

//#include <windows.h>
#include "../../../src/hge/HGEExport.h"
extern "C"
{
#include "../../../src/utility/exception.h"
#include "../../../src/nge_define.h"
#include "../../../src/libnge2.h"
#include "../../../src/nge_main.h"
#include "../../../src/nge_io_file.h"
#include "../../../src/nge_timer.h"
#include "../../../src/nge_graphics.h"
};
#include <list>
#include <vector>
#include "../Header/VectorList.h"
#include "../../../src/hge/hgeEffectSystem.h"
#include "../../../src/hge/hgeFont.h"

using namespace std;
/*
#include "../../../include/hge.h"
#include "../../../include/hgefont.h"
#include "../../../include/hgesprite.h"
#include "../../../include/hgeeffectsystem.h"
*/

#include "BMath.h"

#pragma warning(disable:4244)
#pragma warning(disable:4800)
#pragma warning(disable:1786)
/*
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
	#pragma comment(lib, "hgehelp_d.lib")
#else
	#pragma comment(lib, "hgehelp.lib")
#endif // _DEBUG

#ifdef _DEBUG
	#pragma comment(lib, "hge_d.lib")
#else
	#pragma comment(lib, "hge.lib")
#endif // _DEBUG

#pragma comment(lib, "kailleraclient.lib")
#pragma comment(lib, "bass.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
*/
#endif