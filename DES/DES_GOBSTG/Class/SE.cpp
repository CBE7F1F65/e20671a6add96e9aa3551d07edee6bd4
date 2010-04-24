#include "../Header/SE.h"
#include "../Header/BResource.h"
#include "../../../src/hge/HGEExport.h"
//#include "../../../src/core/BASS/bass.h"

//pan coefficient
#define M_SE_PAN 0.35f

#define SE_FREEBEGIN		0x40

SE SE::se[SEMAX];
BYTE SE::vol;

bool SE::Initial()
{
	return true;

	char buffer[M_STRMAX];
	for(int i = 0;i < SEMAX;i++)
	{
		strcpy(buffer, BResource::pbres->resdata.sefilename[i]);
		if(!strlen(buffer))
		{
//			if(i < SE_FREEBEGIN)
//				strcpy(buffer, BResource::res.resdata.sefilename[SE_DEFAULT]);
//			else
				continue;
		}
		se[i].eff = hge->Effect_Load(buffer);
		if(se[i].eff == NULL)
		{
#ifdef __DEBUG
			HGELOG("%s\nFailed in loading SE File %s.(To be assigned to Index %d).", HGELOG_ERRSTR, buffer, i);
#endif
//			return false;
		}
#ifdef __DEBUG
		else
		{
			HGELOG("Succeeded in loading SE File %s.(Assigned to Index %d).", buffer, i);
		}
#endif
		se[i].chn = NULL;
		se[i].counter = 0;
		se[i].sum = 0;
	}
	return true;
}

bool SE::have(BYTE type)
{
	if (se[type].counter)
	{
		return true;
	}
	return false;
}

void SE::push(BYTE type, float x)
{
	se[type].counter++;
	se[type].sum += x - M_CLIENT_CENTER_X;
}

void SE::play()
{
	for(int i = 0;i < SEMAX;i++)
	{
		if(se[i].counter)
		{
			int pan = (int)((se[i].sum / se[i].counter) * M_SE_PAN);

//			if(hge->Channel_IsPlaying(se[i].chn))
//				hge->Channel_SetPos(se[i].chn, 0);
//			else
				if (se[i].eff)
			{
				se[i].chn = hge->Effect_PlayEx(se[i].eff, vol/3, pan);
			}
				
		}
		se[i].counter = 0;
		se[i].sum = 0;
	}
}

void SE::stop(int type)
{
	if (type < 0)
	{
		for (int i=0; i<SEMAX; i++)
		{
			stop(i);
		}
		return;
	}
	se[type].counter = 0;
	se[type].sum = 0;
	hge->Channel_Stop(se[type].chn);
}