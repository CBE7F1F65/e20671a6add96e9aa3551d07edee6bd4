#include "../header/Fontsys.h"
#include "../header/Main.h"
#include "../header/Scripter.h"
#include "../header/Data.h"
#include "../header/SpriteItemManager.h"
#include "../../../src/hge/HGEExport.h"

list<Fontsys *> Fontsys::fontsys;
HD3DFONT Fontsys::font = NULL;

Fontsys::Fontsys()
{
	tar = NULL;
	sprite = NULL;
	signedup = false;
//	ZeroMemory(&quad, sizeof(hgeQuad));
	strcpy(text, "");
}

Fontsys::~Fontsys()
{
	ReleaseTargetAndSprite();
}

void Fontsys::ReleaseTargetAndSprite()
{

	if (tar)
	{
		hge->Target_Free(tar);
		tar = NULL;
	}

	SpriteItemManager::FreeSprite(&sprite);
}

bool Fontsys::SignOff(bool erase)
{
	ReleaseTargetAndSprite();
	strcpy(text, "");
	if (!erase || !signedup)
	{
		return true;
	}
	for (list<Fontsys *>::iterator it=fontsys.begin(); it!= fontsys.end(); it++)
	{
		if (*it == this)
		{
			it = fontsys.erase(it);
			return true;
		}
	}
//	ZeroMemory(&quad, sizeof(hgeQuad));
	return false;
}

void Fontsys::Release()
{
	for (list<Fontsys *>::iterator it=fontsys.begin(); it!=fontsys.end();)
	{
		if ((*it)->SignOff(false))
		{
			it = fontsys.erase(it);
		}
		else
		{
			it++;
		}
	}
	fontsys.clear();
}

void Fontsys::Init(HD3DFONT _font)
{
	if (_font != NULL)
	{
		font = _font;
	}
	Release();
}

void Fontsys::HeatUp()
{
	/*
	for (int i=0; i<scr.strdescIndex; i+=(i+1))
	{
		SignUp(FONTSYS_CHATUSE, strdesc[i]);
	}
	*/
	/*
	Fontsys _fs;
	for (int i=0; i<PLAYERTYPEMAX; i++)
	{
		_fs.SignUp(data.getPlayerName(i));
	}
	for (int i=0; i<ENEMYTYPEMAX; i++)
	{
		_fs.SignUp(data.getEnemyName(i));
	}
	_fs.SignOff();
	*/
}

bool Fontsys::GfxRestore()
{
	for (list<Fontsys *>::iterator it=fontsys.begin(); it!=fontsys.end(); it++)
	{
		HD3DFONT _usingfont = (*it)->usingfont;
		(*it)->SignUp(NULL, _usingfont);
		(*it)->sprite->SetTexture(hge->Target_GetTexture((*it)->tar));
	}
	return true;
}

int Fontsys::strTranslate(char * dtext, const char * stext, int * maxchar)
{
	int _lines = 1;
	int _maxcharinline = -1;
	int lastj = 0;
	int j=0;
	strcpy(dtext, "");
	for(int i=0; i<(int)strlen(stext); i++)
	{
		if (stext[i] == '\n')
		{
			if (_maxcharinline < j - lastj)
			{
				_maxcharinline = j - lastj;
			}
			lastj = j;
			_lines++;
		}
		if (stext[i]<0)
		{
			char twchar[3];
			twchar[0] = stext[i];
			i++;
			j+=2;
			twchar[1] = stext[i];
			twchar[2] = 0;
			strcat(dtext, twchar);
			continue;
		}
		if(stext[i] == FONTSYS_TRANSCHAR)
		{
			i++;
			if(stext[i] != FONTSYS_TRANSCHAR)
			{
				int strcodecon = stext[i] - '0';
				if(!strcodecon)
				{
					int strcodeval = (stext[i+1] - '0') * 10 +	(stext[i+2] - '0');
					while(j < strcodeval)
					{
						strcat(dtext, " ");
						j++;
					}
					i += 2;
					continue;
				}
				else
				{
					int strcodeval1 = (stext[i+1] - '0') * 10 +	(stext[i+2] - '0');
					int strcodeval2 = (stext[i+3] - '0') * 10 +	(stext[i+4] - '0');
					while(j < strcodeval1)
					{
						strcat(dtext, " ");
						j++;
					}
					i += 4;
					char buffer[32];
					int k = 0;
					while(!(stext[i+1] == FONTSYS_TRANSCHAR && stext[i+2] == FONTSYS_CONTROLCHAR))
					{
						i++;
						buffer[k] = stext[i];
						k++;
					}
					buffer[k] = 0;
					for(int l=k;l<strcodeval2-strcodeval1;l++)
					{
						if(strcodecon == 2)
							strcat(dtext, "0");
						else
							strcat(dtext, " ");
						j++;
					}
					strcat(dtext, buffer);
					i += 2;
					j += k;
					continue;
				}
			}
		}
		char tbuff[2];
		tbuff[0] = stext[i];
		tbuff[1] = 0;
		strcat(dtext, tbuff);
		j++;
	}
	if (_maxcharinline < j - lastj)
	{
		_maxcharinline = j - lastj;
	}
	if (maxchar)
	{
		*maxchar = _maxcharinline;
	}
	return _lines;
}

void Fontsys::SignUp(const char * _text, HD3DFONT _font)
{
	HTEXTURE tex;

	if (_text != NULL)
	{
		SignOff();
		lines = strTranslate(text, _text, &maxcharinline);
	}
	ReleaseTargetAndSprite();
	tar = NULL;//hge->Target_Create(FONTSYS_TARGETWIDTH, FONTSYS_TARGETHEIGHT, false);
	if (!_font)
	{
		_font = font;
	}
	usingfont = _font;
	if (!usingfont)
	{
		return;
	}

	tex = hge->Texture_Create(maxcharinline*25, lines*25);
	char temptext[M_STRMAX];
	strcpy(temptext, text);

	int fontheight = 0;
	while (strlen(temptext))
	{
		char tsubtext[M_STRMAX];
		int i=0;
		for (; i<strlen(temptext); i++)
		{
			if (temptext[i] == '\n')
			{
				break;
			}
			tsubtext[i] = temptext[i];
		}
		tsubtext[i] = 0;
		fontheight += hge->Gfx_RenderTextToTarget(&tex, &tar, _font, tsubtext, 0, fontheight, FONTSYS_TARGETWIDTH, FONTSYS_TARGETHEIGHT);
		if (i >= strlen(temptext))
		{
			break;
		}
		strcpy(tsubtext, &temptext[i+1]);
		strcpy(temptext, tsubtext);
	}

//	float w = strlen(text) * M_FONTWIDTH;
//	float h = lines * M_FONTHEIGHT;

	float w = (fontheight / lines) * 0.55f * (maxcharinline+1);
	float h = fontheight;
	if (w > FONTSYS_TARGETWIDTH)
	{
		w = FONTSYS_TARGETWIDTH;
	}
	if (h > FONTSYS_TARGETHEIGHT)
	{
		h = FONTSYS_TARGETHEIGHT;
	}
//	float tx = w / FONTSYS_TARGETWIDTH;
//	float ty = h / FONTSYS_TARGETHEIGHT;

	if (tex)
	{
		sprite = SpriteItemManager::CreateNullSprite();
		SpriteItemManager::SetSpriteData(sprite, tex, 0, 0, w, h, false, false, true);
		sprite->SetBlendMode(BLEND_DEFAULT);
	}

	/*
	quad.tex = tex;
	quad.blend = BLEND_DEFAULT;
	quad.v[0].tx = 0;	quad.v[0].ty = 0;
	quad.v[1].tx = tx;	quad.v[1].ty = 0;
	quad.v[2].tx = tx;	quad.v[2].ty = ty;
	quad.v[3].tx = 0;	quad.v[3].ty = ty;
	quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0;
	quad.v[0].x = 0;					quad.v[0].y = 0;
	quad.v[1].x = w;	quad.v[1].y = 0;
	quad.v[2].x = w;	quad.v[2].y = h;
	quad.v[3].x = 0;	quad.v[3].y = h;
	*/

	if (_text)
	{
		fontsys.push_back(this);
	}

	signedup = true;
}

void Fontsys::Render(float x, float y, float shadow, float hscale, float vscale, BYTE alignflag)
{
	if (!sprite)
	{
		return;
	}
	if (shadow)
	{
		DWORD _col[4];
		for (int i=0; i<4; i++)
		{
			_col[i] = col[i];
		}
		DWORD __col[4];
		for (int i=0; i<4; i++)
		{
			__col[i] = _col[i] & 0xFF000000;
		}
		SetColor(__col[0], __col[1], __col[2], __col[3]);
		Render(x+shadow*2, y+shadow*2, 0, hscale, vscale, alignflag);

		for (int i=0; i<4; i++)
		{
			__col[i] = (GETA(_col[i]) / 4) << 24 | (_col[i] & 0xFFFFFF);
		}

		SetColor(__col[0], __col[1], __col[2], __col[3]);
		Render(x+shadow, y+shadow, 0, hscale, vscale, alignflag);
		Render(x+shadow, y-shadow, 0, hscale, vscale, alignflag);
		Render(x-shadow, y-shadow, 0, hscale, vscale, alignflag);
		Render(x-shadow, y+shadow, 0, hscale, vscale, alignflag);
		SetColor(_col[0], _col[1], _col[2], _col[3]);
	}
	else
	{
		SetColor(col[0], col[1], col[2], col[3]);
	}
	float hotx;
	float hoty;
	float w = sprite->GetWidth();
	float h = sprite->GetHeight();
	switch (alignflag & HGETEXT_HORZMASK)
	{
	case HGETEXT_LEFT:
		hotx = 0;
		break;
	case HGETEXT_CENTER:
		hotx = w / 2;
		break;
	case HGETEXT_RIGHT:
		hotx = w;
		break;
	}
	switch (alignflag & HGETEXT_VERTMASK)
	{
	case HGETEXT_TOP:
		hoty = 0;
		break;
	case HGETEXT_MIDDLE:
		hoty = h / 2;
		break;
	case HGETEXT_BOTTOM:
		hoty = h;
		break;
	}
	sprite->SetColor(col[0], col[1], col[2], col[3]);
	SpriteItemManager::SetSpriteHotSpot(sprite, hotx, hoty);
//	sprite->SetHotSpot(hotx, hoty);
	sprite->RenderEx(x, y, 0, hscale, vscale);
	/*
	float w = quad.v[1].x - quad.v[0].x;
	float h = quad.v[2].y - quad.v[0].y;

	quad.v[0].col = quad.v[1].col = ucol;
	quad.v[2].col = quad.v[3].col = dcol;

	quad.v[0].x = x - hext;		quad.v[0].y = y - vext;
	quad.v[1].x = x + w + hext;	quad.v[1].y = y - vext;
	quad.v[2].x = x + w + hext;	quad.v[2].y = y + h + vext;
	quad.v[3].x = x - hext;		quad.v[3].y = y + h + vext;

	Export::Gfx_RenderQuad(&quad);

	quad.v[0].x = x;		quad.v[0].y = y;
	quad.v[1].x = x + w;	quad.v[1].y = y;
	quad.v[2].x = x + w;	quad.v[2].y = y + h;
	quad.v[3].x = x;		quad.v[3].y = y + h;
	*/
}

void Fontsys::SetColor(DWORD _col, int i)
{
	if (i < 0)
	{
		SetColor(_col, _col, _col, _col);
	}
	else
	{
		col[i] = _col;
	}
}

void Fontsys::SetColor(DWORD col0, DWORD col1, DWORD col2, DWORD col3)
{
	col[0] = col0;
	col[1] = col1;
	col[2] = col2;
	col[3] = col3;
}