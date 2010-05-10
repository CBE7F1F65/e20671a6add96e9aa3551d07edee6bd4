#include "../../../src/hge/HGEExport.h"
#include "../Header/SpriteItemManager.h"
#include "../Header/FrontDisplayName.h"
#include "../Header/BResource.h"

hgeTextureInfo * SpriteItemManager::texinfo;
int SpriteItemManager::nullIndex = 0;
int SpriteItemManager::yesIndex = 0;
int SpriteItemManager::noIndex = 0;
int SpriteItemManager::cancelIndex = 0;
int SpriteItemManager::confirmIndex = 0;

FrontSprite SpriteItemManager::frontsprite[FRONTSPRITEMAX];

SpriteItemManager::SpriteItemManager()
{
	ZeroMemory(frontsprite, sizeof(FrontSprite) * FRONTSPRITEMAX);
}

SpriteItemManager::~SpriteItemManager()
{

}

void SpriteItemManager::Init(hgeTextureInfo * _texinfo)
{
	texinfo = _texinfo;
	FreeFrontSprite();
}

void SpriteItemManager::Release()
{
	FreeFrontSprite();
}

HTEXTURE SpriteItemManager::GetTexture(int index)
{
	if (index < 0 || index >= BResource::pbres->spritenumber || !texinfo)
	{
		return NULL;
	}
	return BResource::pbres->spritedata[index].tex;
}

float SpriteItemManager::GetTexX(int index)
{
	if (index < 0 || index >= BResource::pbres->spritenumber)
	{
		return 0;
	}
	return BResource::pbres->spritedata[index].tex_x;
}

float SpriteItemManager::GetTexY(int index)
{
	if (index < 0 || index >= BResource::pbres->spritenumber)
	{
		return 0;
	}
	return BResource::pbres->spritedata[index].tex_y;
}

float SpriteItemManager::GetTexW(int index)
{
	if (index < 0 || index >= BResource::pbres->spritenumber)
	{
		return 1;
	}
	return BResource::pbres->spritedata[index].tex_w;
}

float SpriteItemManager::GetTexH(int index)
{
	if (index < 0 || index >= BResource::pbres->spritenumber)
	{
		return 1;
	}
	return BResource::pbres->spritedata[index].tex_h;
}

void SpriteItemManager::FrontSpriteAction()
{
	for (int i=0; i<FRONTSPRITEMAX; i++)
	{
		if (frontsprite[i].sprite)
		{
			if (frontsprite[i].fadeoutmaxtime)
			{
				frontsprite[i].fadeouttimer++;
				if (frontsprite[i].fadeouttimer == frontsprite[i].fadeoutmaxtime)
				{
					FreeFrontSprite(i);
				}
			}
		}
	}
}

void SpriteItemManager::RenderFrontSprite()
{
	for (int i=0; i<FRONTSPRITEMAX; i++)
	{
		if (frontsprite[i].sprite)
		{
			if (frontsprite[i].fadeoutmaxtime)
			{
				BYTE _alpha = INTER(0xff, 0, ((float)frontsprite[i].fadeouttimer)/frontsprite[i].fadeoutmaxtime);
				frontsprite[i].col = (_alpha<<24)|0xffffff;
			}
			frontsprite[i].sprite->SetColor(frontsprite[i].col);
			frontsprite[i].sprite->SetBlendMode(frontsprite[i].blend);
			frontsprite[i].sprite->RenderEx(frontsprite[i].x, frontsprite[i].y, frontsprite[i].rot, frontsprite[i].hscale, frontsprite[i].vscale);
		}
	}
}

int SpriteItemManager::GetIndexByName(const char * spritename)
{
	if (!spritename)
	{
		return -1;
	}
	if (!strlen(spritename))
	{
		return -1;
	}
	for (int i=0; i<BResource::pbres->spritenumber; i++)
	{
		if (!strcmp(spritename, BResource::pbres->spritedata[i].spritename))
		{
			return i;
			break;
		}
	}
	return -1;
}

spriteData * SpriteItemManager::CastSprite(int index)
{
	if (index >= 0 && index < BResource::pbres->spritenumber)
	{
		return &(BResource::pbres->spritedata[index]);
	}
	return NULL;
}

bool SpriteItemManager::SetSprite(int index, hgeSprite * sprite)
{
	if (!sprite)
	{
		return false;
	}
	if (index < 0)
	{
		SetSpriteTextureRect(sprite, 0, 0, 0, 0);
		return true;
	}
	spriteData * _sd = CastSprite(index);
	if (!_sd)
	{
		return false;
	}
	SetSpriteData(sprite, _sd->tex, _sd->tex_x, _sd->tex_y, 
		_sd->tex_w < 0 ? hge->Texture_GetWidth(_sd->tex)-_sd->tex_x : _sd->tex_w, 
		_sd->tex_h < 0 ? hge->Texture_GetHeight(_sd->tex)-_sd->tex_y : _sd->tex_h);
	return true;
}

bool SpriteItemManager::HaveFrontSprite(int ID)
{
	if (ID < 0 || ID >= FRONTSPRITEMAX)
	{
		return false;
	}
	if (frontsprite[ID].sprite)
	{
		return true;
	}
	return false;
}

void SpriteItemManager::ClearFrontSpriteVale(int ID)
{
	if (ID < 0 || ID >= FRONTSPRITEMAX)
	{
		return;
	}
	hgeSprite * _sprite = frontsprite[ID].sprite;
	ZeroMemory(&frontsprite[ID], sizeof(FrontSprite));
	frontsprite[ID].blend = BLEND_DEFAULT;
	frontsprite[ID].sprite = _sprite;
}

hgeSprite * SpriteItemManager::ChangeFrontSprite(int ID, int index)
{
	if (ID < 0 || ID >= FRONTSPRITEMAX)
	{
		return NULL;
	}
	if (HaveFrontSprite(ID))
	{
		if (ChangeSprite(index, frontsprite[ID].sprite))
		{
			return frontsprite[ID].sprite;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		frontsprite[ID].sprite = CreateSprite(index);
		return frontsprite[ID].sprite;
	}
	return NULL;

}

void SpriteItemManager::FreeFrontSprite(int ID/* =-1 */)
{
	if (ID >= FRONTSPRITEMAX)
	{
		return;
	}
	if (ID < 0)
	{
		for (int i=0; i<FRONTSPRITEMAX; i++)
		{
			FreeFrontSprite(i);
		}
		return;
	}
	FreeSprite(&frontsprite[ID].sprite);
	ClearFrontSpriteVale(ID);
}

hgeSprite * SpriteItemManager::BuildFrontSprite(int ID, int index)
{
	if (index >= 0)
	{
		if (HaveFrontSprite(ID))
		{
			FreeFrontSprite(ID);
		}
		return ChangeFrontSprite(ID, index);
	}
	return NULL;
}

hgeSprite * SpriteItemManager::BuildFrontSpriteByName(int ID, const char * spritename)
{
	return BuildFrontSprite(ID, GetIndexByName(spritename));
}

void SpriteItemManager::SetFrontSpriteValue(int ID, float x, float y, int angle/* =0 */, float hscale/* =1.0f */, float vscale/* =0.0f */, DWORD col/* =0xffffffff */, int blend/* =BLEND_DEFAULT */)
{
	if (ID < 0 || ID >= FRONTSPRITEMAX)
	{
		return;
	}
	frontsprite[ID].x = x;
	frontsprite[ID].y = y;
	frontsprite[ID].rot = ARC(angle);
	frontsprite[ID].hscale = hscale;
	frontsprite[ID].vscale = vscale;
	frontsprite[ID].col = col;
	frontsprite[ID].blend = blend;
}

void SpriteItemManager::SetFrontSpriteFadeoutTime(int ID, int fadeouttime)
{
	if (ID < 0 || ID >= FRONTSPRITEMAX)
	{
		return;
	}
	frontsprite[ID].fadeoutmaxtime = fadeouttime;
}

hgeSprite * SpriteItemManager::CreateNullSprite()
{
	hgeSprite * sprite;
	if (!texinfo)
	{
		return NULL;
	}
	sprite = CreateSprite(nullIndex);
	return sprite;
}

hgeSprite * SpriteItemManager::CreateSprite(int index)
{
	hgeSprite * sprite;
	if (!texinfo)
	{
		return NULL;
	}
//	sprite = CreateNullSprite();
	sprite = new hgeSprite();
	if (index < 0)
	{
		return sprite;
	}
//	SetSpriteData(sprite, tex[BResource::pbres->spritedata[index].tex], BResource::pbres->spritedata[index].tex_x, BResource::pbres->spritedata[index].tex_y, BResource::pbres->spritedata[index].tex_w, BResource::pbres->spritedata[index].tex_h);
	SetSpriteData(sprite, BResource::pbres->spritedata[index].tex, BResource::pbres->spritedata[index].tex_x, BResource::pbres->spritedata[index].tex_y, BResource::pbres->spritedata[index].tex_w, BResource::pbres->spritedata[index].tex_h);
	return sprite;
}

bool SpriteItemManager::SetSpriteData(hgeSprite * sprite, HTEXTURE _tex, float texx, float texy, float texw, float texh, bool flipx/* =false */, bool flipy/* =false */, bool externtex/* =false */)
{
	if (!sprite || !texinfo)
	{
		return false;
	}
/*
	if (!tex[_tex])
	{
//		_tex = tex[TEX_WHITE];
		_tex = TEX_WHITE;
	}*/

	sprite->SetTexture(_tex, externtex);
	if (!SetSpriteTextureRect(sprite, texx, texy, texw, texh))
	{
		return false;
	}
	if (!SetSpriteFlip(sprite, flipx, flipy))
	{
		return false;
	}
	return true;
}

bool SpriteItemManager::SetSpriteTextureRect(hgeSprite * sprite, float texx, float texy, float texw, float texh)
{
	if (!sprite)
	{
		return false;
	}
	sprite->SetTextureRect(texx, texy, texw, texh);
	return true;
}

bool SpriteItemManager::SetSpriteHotSpot(hgeSprite * sprite, float hotx, float hoty)
{
	if (!sprite)
	{
		return false;
	}
	sprite->SetHotSpot(hotx, hoty);
	return true;
}

bool SpriteItemManager::SetSpriteFlip(hgeSprite * sprite, bool flipx/* =false */, bool flipy/* =false */)
{
	if (!sprite)
	{
		return false;
	}
	sprite->SetFlip(flipx, flipy);
	return true;
}

bool SpriteItemManager::CreateSprite(int index, hgeSprite ** sprite)
{
	if (!sprite)
	{
		return false;
	}
	*sprite = CreateSprite(index);
	if (*sprite)
	{
		return true;
	}
	return false;
}

bool SpriteItemManager::ChangeSprite(int index, hgeSprite * sprite)
{
	if (!sprite || index < 0)
	{
		return false;
	}
	SetSpriteData(sprite, BResource::pbres->spritedata[index].tex, BResource::pbres->spritedata[index].tex_x, BResource::pbres->spritedata[index].tex_y, BResource::pbres->spritedata[index].tex_w, BResource::pbres->spritedata[index].tex_h);
	return true;
}

hgeSprite * SpriteItemManager::CreateSpriteByName(const char * spritename)
{
	int index = GetIndexByName(spritename);
	if (index >= 0)
	{
		return CreateSprite(index);
	}
	return NULL;
}

bool SpriteItemManager::CreateSpriteByName(const char * spritename, hgeSprite ** sprite)
{
	if (!sprite)
	{
		return false;
	}
	*sprite = CreateSpriteByName(spritename);
	if (*sprite)
	{
		return true;
	}
	return false;
}

void SpriteItemManager::FreeSprite(hgeSprite ** sprite)
{
	if (sprite && *sprite)
	{
		free(*sprite);
		*sprite = NULL;
	}
}

bool SpriteItemManager::ptFace(int index, hgeSprite * sprite)
{
	if (!sprite || !texinfo)
	{
		return false;
	}
	if (index < 0)
	{
		SpriteItemManager::SetSpriteTextureRect(sprite, 0, 0, 0, 0);
		return true;
	}
	ChangeSprite(BResource::pbres->playerdata[index].faceSIID, sprite);
	return true;
}
/*
bool SpriteItemManager::ptName(int index, hgeSprite * sprite, bool enemy)
{
	if (!sprite || !tex)
	{
		return false;
	}
	if (index < 0)
	{
		sprite->SetTextureRect(0, 0, 0, 0);
		return true;
	}
	spriteData * tsd = &(BResource::res.spritedata[index+(enemy?nameIndexEnemy:nameIndexPlayer)]);
	sprite->SetTexture(tex[tsd->tex]);
	sprite->SetTextureRect(tsd->tex_x, tsd->tex_y, tsd->tex_w, tsd->tex_h);
	return true;
}
*/