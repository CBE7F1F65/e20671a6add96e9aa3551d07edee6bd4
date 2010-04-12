#include "../Header/Process.h"
#include "../Header/Scripter.h"

int Process::processPlayerSelect()
{
	gametime++;
	if(gametime == 1)
	{
		Scripter::scr.Execute(SCR_EVENT, SCR_EVENT_ENTERSTATE, STATE_PLAYER_SELECT);
	}
	retvalue = PGO;
	Scripter::scr.Execute(SCR_CONTROL, STATE_PLAYER_SELECT, gametime);
	return retvalue;
}
