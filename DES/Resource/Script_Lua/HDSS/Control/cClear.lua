function CEClear_CloseUsed()
end

function CEClear_Init()
end

function CEClear_ExitState(tostate)
	CEClear_CloseUsed();
	hdssSETSTATE(tostate);
	hdssCLEARALL();
end

function ControlExecute_cClear(timer)
	
	local chati = hdss.Get(HDSS_CHATI);
	local winner = 0;
	local csbegin = CS_L;
	local csafter = CS_R;
	if hdss.Get(HDSS_PLIFE, 1) > 0 then
		winner = 1;
		csbegin = CS_R;
		csafter = CS_L;
	end
	
	if timer == 1 then
		CEClear_Init();
		--
		if _DEBUG_MatchAndLog > 0 then
			local filename = _DEBUG_GetSaveReplayName();
			hdssSAVEREPLAY(true, true, filename);
			local logstr = "SR :	"..filename;
			LOG(logstr);
			
			_DEBUG_NewRandomMatch();
			
			return true;
			
		end
		--
		
		for i=0, 1 do
			local siadd = 1;
			if i == winner then
				siadd = 0;
			end
			hdssBGVALUE(i, LConst_gamefg_infoid, SI_GameInfo_Winner+siadd, helper_GetCenterX(i), CenterY-100);
			hdssFRONTSPRITE(LConst_game_FrontSprite_WinLoseID_0+i, SI_GameInfo_GameSet, helper_GetCenterX(i), TotalCenterY-160);
			hdssFADEOUTFRONTSPRITE(LConst_game_FrontSprite_WinLoseID_0+i, 64);
		end
	end
		
	
	if timer == 120 then
		CEClear_ExitState(STATE_OVER);
	end
end