#ifndef __NOTUSELUA

#include "../Header/Export_Lua_HGEHelp.h"
#include "../Header/LuaConstDefine.h"
#include "../Header/Export.h"
#include "../../../src/hge/HGEExport.h"

bool Export_Lua_HGEHelp::_LuaRegistFunction(LuaObject * obj)
{
	return true;
}

bool Export_Lua_HGEHelp::_LuaRegistConst(LuaObject * obj)
{
	// hgeFont
	obj->SetInteger("HGETEXT_LEFT", HGETEXT_LEFT);
	obj->SetInteger("HGETEXT_RIGHT", HGETEXT_RIGHT);
	obj->SetInteger("HGETEXT_CENTER", HGETEXT_CENTER);
	obj->SetInteger("HGETEXT_HORZMASK", HGETEXT_HORZMASK);

	obj->SetInteger("HGETEXT_TOP", HGETEXT_TOP);
	obj->SetInteger("HGETEXT_BOTTOM", HGETEXT_BOTTOM);
	obj->SetInteger("HGETEXT_MIDDLE", HGETEXT_MIDDLE);
	obj->SetInteger("HGETEXT_VERTMASK", HGETEXT_VERTMASK);

	return true;
}


#endif