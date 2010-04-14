#ifndef __NOTUSELUA

#include "../Header/Export_Lua.h"
#include "../Header/LuaConstDefine.h"
#include "../Header/BResource.h"

bool Export_Lua::_LuaRegistConst(LuaObject * obj)
{
	// System
	obj->SetInteger("NULL",	NULL);
#ifdef __DEBUG
	obj->SetBoolean("_DEBUG", true);
#else
	obj->SetBoolean("_DEBUG", false);
#endif // __DEBUG

	// DI
	obj->SetInteger("PSP_BUTTON_UP", PSP_BUTTON_UP);
	obj->SetInteger("PSP_BUTTON_DOWN", PSP_BUTTON_DOWN);
	obj->SetInteger("PSP_BUTTON_LEFT", PSP_BUTTON_LEFT);
	obj->SetInteger("PSP_BUTTON_RIGHT", PSP_BUTTON_RIGHT);
	obj->SetInteger("PSP_BUTTON_TRIANGLE", PSP_BUTTON_TRIANGLE);
	obj->SetInteger("PSP_BUTTON_CIRCLE", PSP_BUTTON_CIRCLE);
	obj->SetInteger("PSP_BUTTON_CROSS", PSP_BUTTON_CROSS);
	obj->SetInteger("PSP_BUTTON_SQUARE", PSP_BUTTON_SQUARE);
	obj->SetInteger("PSP_BUTTON_LEFT_TRIGGER", PSP_BUTTON_LEFT_TRIGGER);
	obj->SetInteger("PSP_BUTTON_RIGHT_TRIGGER", PSP_BUTTON_RIGHT_TRIGGER);
	obj->SetInteger("PSP_BUTTON_SELECT", PSP_BUTTON_SELECT);
	obj->SetInteger("PSP_BUTTON_START", PSP_BUTTON_START);
	obj->SetInteger("PSP_BUTTON_HOME", PSP_BUTTON_HOME);
	obj->SetInteger("PSP_BUTTON_HOLD", PSP_BUTTON_HOLD);

	return true;
}

bool Export_Lua::_LuaRegistCustomConst(LuaObject * obj)
{
	// CustomConst
	for (int i=0; i<SCR_CUSTOMCONSTMAX; i++)
	{
		if (strlen(BResource::pbres->customconstdata[i].name))
		{
			obj->SetInteger(BResource::pbres->customconstdata[i].name, BResource::pbres->customconstdata[i].value);
		}
	}

	return true;
}

#endif