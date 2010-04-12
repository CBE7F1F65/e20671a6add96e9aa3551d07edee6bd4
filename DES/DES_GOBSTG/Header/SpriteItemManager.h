#ifndef _SPRITEITEMMANAGER_H
#define _SPRITEITEMMANAGER_H

#include "MainDependency.h"
#include "DataStruct.h"

#define FRONTSPRITEMAX		0x40

struct FrontSprite
{
	hgeSprite * sprite;
	float x;
	float y;
	float rot;
	float hscale;
	float vscale;
	DWORD col;
	int blend;
	int fadeoutmaxtime;
	int fadeouttimer;
};

class SpriteItemManager
{
public:
	SpriteItemManager();
	~SpriteItemManager();

	static void Init(HTEXTURE * tex);
	static void Release();
	static hgeSprite * CreateSprite(int index);
	static bool CreateSprite(int index, hgeSprite ** sprite);
	static bool ChangeSprite(int index, hgeSprite * sprite);
	static int GetIndexByName(const char * spritename);
	static hgeSprite * CreateSpriteByName(const char * spritename);
	static bool CreateSpriteByName(const char * spritename, hgeSprite ** sprite);
	static void FreeSprite(hgeSprite ** sprite);
	static spriteData * CastSprite(int index);

	static hgeSprite * CreateNullSprite();
	static bool SetSpriteData(hgeSprite * sprite, HTEXTURE tex, float texx, float texy, float texw, float texh, bool flipx=false, bool flipy=false);
	static bool SetSpriteTextureRect(hgeSprite * sprite, float texx, float texy, float texw, float texh);
	static bool SetSpriteFlip(hgeSprite * sprite, bool flipx=false, bool flipy=false);
	static bool SetSpriteHotSpot(hgeSprite * sprite, float hotx, float hoty);

	static bool HaveFrontSprite(int ID);
	static void ClearFrontSpriteVale(int ID);
	static hgeSprite * ChangeFrontSprite(int ID, int index);
	static hgeSprite * BuildFrontSprite(int ID, int index);
	static hgeSprite * BuildFrontSpriteByName(int ID, const char * spritename);
	static void SetFrontSpriteValue(int ID, float x, float y, int angle=0, float hscale=1.0f, float vscale=0.0f, DWORD col=0xffffffff, int blend=BLEND_DEFAULT);
	static void SetFrontSpriteFadeoutTime(int ID, int fadeouttime);
	static void FrontSpriteAction();
	static void FreeFrontSprite(int ID=-1);
	static void RenderFrontSprite();

	static bool SetSprite(int index, hgeSprite * sprite, HTEXTURE * tex);

	static HTEXTURE GetTexture(int index);
	static float GetTexX(int index);
	static float GetTexY(int index);
	static float GetTexW(int index);
	static float GetTexH(int index);

	static bool ptFace(int index, hgeSprite * sprite);

public:
	static HTEXTURE * tex;

	static int yesIndex;
	static int noIndex;
	static int cancelIndex;
	static int confirmIndex;
	static int nullIndex;

	static FrontSprite frontsprite[FRONTSPRITEMAX];
};

#endif