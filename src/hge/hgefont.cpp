/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeFont helper class implementation
*/


#include "hgefont.h"
#include <stdlib.h>
#include <stdio.h>
#include "../nge_io_file.h"
#include "HGEExport.h"

const char FNTHEADERTAG[]="[HGEFONT]";
const char FNTBITMAPTAG[]="Bitmap";
const char FNTCHARTAG[]="Char";


char hgeFont::buffer[1024];


/************************************************************************/
/* This function is added by h5nc (h5nc@yahoo.com.cn)                   */
/************************************************************************/
void hgeFont::_FontInit()
{
	fHeight=0.0f;
	fScale=1.0f;
	fProportion=1.0f;
	fRot=0.0f;
	fTracking=0.0f;
	fSpacing=1.0f;
	hTexture=0;

	fZ=0;
	nBlend=BLEND_DEFAULT;
	for (int i=0; i<4; i++)
	{
		col[i] = 0xFFFFFFFF;
	}

	ZeroMemory( &letters, sizeof(letters) );
	ZeroMemory( &pre, sizeof(letters) );
	ZeroMemory( &post, sizeof(letters) );
}

/************************************************************************/
/* This function is added by h5nc (h5nc@yahoo.com.cn)                   */
/************************************************************************/
void hgeFont::NewFont(const char *szFont, bool bMipmap/* =false */)
{
	BYTE *	content = NULL;
	DWORD	size;
	char	*desc, *pdesc;
	char	linebuf[256];
	char	buf[MAX_PATH], *pbuf;
	char	chr;
	int		i, x, y, w, h, a, c;

	// Setup variables
	
	// Load font description

	content=hge->Resource_Load(szFont, &size);
	if(!content) return;

	desc = new char[size+1];
	memcpy(desc, content, size);
	desc[size]=0;
	hge->Resource_Free(content);

	pdesc=_get_line(desc,linebuf);
	if(strcmp(linebuf, FNTHEADERTAG))
	{
		delete[] desc;	
		return;
	}

	// Parse font description

	while(pdesc = _get_line(pdesc,linebuf))
	{
		if(!strncmp(linebuf, FNTBITMAPTAG, sizeof(FNTBITMAPTAG)-1 ))
		{
			strcpy(buf,szFont);
			pbuf=strrchr(buf,'\\');
			if(!pbuf) pbuf=strrchr(buf,'/');
			if(!pbuf) pbuf=buf;
			else pbuf++;
			if(!sscanf(linebuf, "Bitmap = %s", pbuf)) continue;

//			hTexture=Export::Texture_Load(buf, 0, bMipmap);
			if(!hTexture)
			{
				delete[] desc;	
				return;
			}
		}

		else if(!strncmp(linebuf, FNTCHARTAG, sizeof(FNTCHARTAG)-1 ))
		{
			pbuf=strchr(linebuf,'=');
			if(!pbuf) continue;
			pbuf++;
			while(*pbuf==' ') pbuf++;
			if(*pbuf=='\"')
			{
				pbuf++;
				i=(unsigned char)*pbuf++;
				pbuf++; // skip "
			}
			else
			{
				i=0;
				while((*pbuf>='0' && *pbuf<='9') || (*pbuf>='A' && *pbuf<='F') || (*pbuf>='a' && *pbuf<='f'))
				{
					chr=*pbuf;
					if(chr >= 'a') chr-='a'-':';
					if(chr >= 'A') chr-='A'-':';
					chr-='0';
					if(chr>0xF) chr=0xF;
					i=(i << 4) | chr;
					pbuf++;
				}
				if(i<0 || i>255) continue;
			}
			sscanf(pbuf, " , %d , %d , %d , %d , %d , %d", &x, &y, &w, &h, &a, &c);

			ChangeSprite(i, hTexture, (float)x, (float)y, (float)w, (float)h, (float)a, (float)c);
			/*
			letters[i] = new hgeSprite(hTexture, (float)x, (float)y, (float)w, (float)h);
			letters[i] -> SetHotSpot(0, 0);
			pre[i]=(float)a;
			post[i]=(float)c;
			if(h>fHeight) fHeight=(float)h;
			*/
		}
	}

	delete[] desc;	
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
hgeFont::hgeFont()
{
	_FontInit();
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
hgeFont::hgeFont(const char *szFont, bool bMipmap)
{
	_FontInit();
	NewFont(szFont, bMipmap);
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
hgeFont::hgeFont(const hgeFont &fnt)
{
	hTexture=fnt.hTexture;
	fHeight=fnt.fHeight;
	fScale=fnt.fScale;
	fRot=fnt.fRot;
	fTracking=fnt.fTracking;
	fSpacing=fnt.fSpacing;
	for (int i=0; i<4; i++)
	{
		col[i] = fnt.col[i];
	}
	fZ=fnt.fZ;
	nBlend=fnt.nBlend;

	for(int i=0; i<256; i++)
	{
		if(fnt.letters[i]) letters[i]=new hgeSprite(*fnt.letters[i]);
		else letters[i]=0;
		pre[i]=fnt.pre[i];
		post[i]=fnt.post[i];
	}
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
hgeFont& hgeFont::operator= (const hgeFont &fnt)
{
	if(this!=&fnt)
	{
		hTexture=fnt.hTexture;
		fHeight=fnt.fHeight;
		fScale=fnt.fScale;
		fRot=fnt.fRot;
		fTracking=fnt.fTracking;
		fSpacing=fnt.fSpacing;
		for (int i=0; i<4; i++)
		{
			col[i] = fnt.col[i];
		}
		fZ=fnt.fZ;
		nBlend=fnt.nBlend;

		for(int i=0; i<256; i++)
		{
			if(letters[i]) delete letters[i];
			if(fnt.letters[i]) letters[i]=new hgeSprite(*fnt.letters[i]);
			else letters[i]=0;
			pre[i]=fnt.pre[i];
			post[i]=fnt.post[i];
		}
	}

	return *this;
}

hgeFont::~hgeFont()
{
	for(int i=0; i<256; i++)
		if(letters[i]) delete letters[i];
//	if(hTexture) Export::Texture_Free(hTexture);
}

/************************************************************************/
/* These functions are added by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
// begin
void hgeFont::ChangeSprite(BYTE chr, hgeSprite * sprite, float pre_a, float post_c)
{
	if (!sprite)
	{
		return;
	}
	float tex_x = 0;
	float tex_y = 0;
	float tex_w = 0;
	float tex_h = 0;
	sprite->GetTextureRect(&tex_x, &tex_y, &tex_w, &tex_h);
	HTEXTURE tex = sprite->GetTexture();
	if (tex)
	{
		ChangeSprite(chr, tex, tex_x, tex_y, tex_w, tex_h, pre_a, post_c);
	}
}

void hgeFont::ChangeSprite(BYTE chr, HTEXTURE tex, float tex_x, float tex_y, float tex_w, float tex_h, float pre_a, float post_c)
{
	if (!letters[chr])
	{
		letters[chr] = new hgeSprite(tex, tex_x, tex_y, tex_w, tex_h);
	}
	else
	{
		letters[chr]->SetTexture(tex);
		letters[chr]->SetTextureRect(tex_x, tex_y, tex_w, tex_h);
	}
	letters[chr]->SetHotSpot(0, 0);
	pre[chr]=pre_a;
	post[chr]=post_c;
	if(tex_h>fHeight) fHeight=tex_h;
}
// end

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
void hgeFont::RenderEx(float x, float y, int align, const char *string, float scale/* =1.0f */, float properation/* =1.0f */, float rotation/* =0 */, float tracking/* =0 */, float spacing/* =1.0f */)
{
	float _scale = fScale;
	float _properation = fProportion;
	float _rotation = fRot;
	float _tracking = fTracking;
	float _spacing = fSpacing;

	SetScale(scale);
	SetProportion(properation);
	SetRotation(rotation);
	SetTracking(tracking);
	SetSpacing(spacing);

	Render(x, y, align, string);

	SetScale(_scale);
	SetProportion(_properation);
	SetRotation(_rotation);
	SetTracking(_tracking);
	SetSpacing(_spacing);
}
/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
void hgeFont::Render(float x, float y, int align, const char *string)
{
	int i;
	float	fx=x;

	align &= HGETEXT_HORZMASK;
	if(align==HGETEXT_RIGHT) fx-=GetStringWidth(string, false);
	if(align==HGETEXT_CENTER) fx-=int(GetStringWidth(string, false)/2.0f);

	while(*string)
	{
		if(*string=='\n')
		{
			y+=int(fHeight*fScale*fSpacing);
			fx=x;
			if(align == HGETEXT_RIGHT)  fx -= GetStringWidth(string+1, false);
			if(align == HGETEXT_CENTER) fx -= int(GetStringWidth(string+1, false)/2.0f);
		}
		else
		{
			i=(unsigned char)*string;
			if(!letters[i]) i='?';
			if(letters[i])
			{
				fx += pre[i]*fScale*fProportion;
				letters[i]->SetColor(col[0], col[1], col[2], col[3]);
				letters[i]->RenderEx(fx, y, fRot, fScale*fProportion, fScale);
				fx += (letters[i]->GetWidth()+post[i]+fTracking)*fScale*fProportion;
			}
		}
		string++;
	}
}

void hgeFont::Printf(float x, float y, int align, const char *format, ...)
{
	char	*pArg=(char *) &format+sizeof(format);

	vsnprintf(buffer, sizeof(buffer)-1, format, pArg);
	buffer[sizeof(buffer)-1]=0;
	//vsprintf(buffer, format, pArg);

	Render(x,y,align,buffer);
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
void hgeFont::Printfb(float x, float y, float w, float h, int align, const char *format, ...)
{
	char	chr, *pbuf, *prevword, *linestart;
	int		i,lines=0;
	float	tx, ty, hh, ww;
	char	*pArg=(char *) &format+sizeof(format);

	vsnprintf(buffer, sizeof(buffer)-1, format, pArg);
	buffer[sizeof(buffer)-1]=0;
	//vsprintf(buffer, format, pArg);

	linestart=buffer;
	pbuf=buffer;
	prevword=0;

	for(;;)
	{
		i=0;
		while(pbuf[i] && pbuf[i]!=' ' && pbuf[i]!='\n') i++;

		chr=pbuf[i];
		pbuf[i]=0;
		ww=GetStringWidth(linestart);
		pbuf[i]=chr;

		if(ww > w)
		{
			if(!chr)
			{
				lines++;
				break;
			}
			if(pbuf==linestart)
			{
				pbuf[i]='\n';
				linestart=&pbuf[i+1];
			}
			else
			{
				*prevword='\n';
				linestart=prevword+1;
				lines++;
			}
		}

		if(pbuf[i]=='\n')
		{
			prevword=&pbuf[i];
			linestart=&pbuf[i+1];
			pbuf=&pbuf[i+1];
			lines++;
			continue;
		}

		if(!pbuf[i]) {lines++;break;}

		prevword=&pbuf[i];
		pbuf=&pbuf[i+1];
	}
	
	tx=x;
	ty=y;
	hh=fHeight*fSpacing*fScale*lines;

	switch(align & HGETEXT_HORZMASK)
	{
		case HGETEXT_LEFT: break;
		case HGETEXT_RIGHT: tx+=w; break;
		case HGETEXT_CENTER: tx+=int(w/2); break;
	}

	switch(align & HGETEXT_VERTMASK)
	{
		case HGETEXT_TOP: break;
		case HGETEXT_BOTTOM: ty+=h-hh; break;
		case HGETEXT_MIDDLE: ty+=int((h-hh)/2); break;
	}

	Render(tx,ty,align,buffer);
}

float hgeFont::GetStringWidth(const char *string, bool bMultiline) const
{
	int i;
	float linew, w = 0;

	while(*string)
	{
		linew = 0;

		while(*string && *string!='\n')
		{
			i=(unsigned char)*string;
			if(!letters[i]) i='?';
				if(letters[i])
					linew += letters[i]->GetWidth() + pre[i] + post[i] + fTracking;

			string++;
		}

		if(!bMultiline) return linew*fScale*fProportion;

		if(linew > w) w = linew;

		while (*string == '\n' || *string == '\r') string++;
  }

	return w*fScale*fProportion;
}


/************************************************************************/
/* This function is added by h5nc (h5nc@yahoo.com.cn)                   */
/************************************************************************/
DWORD hgeFont::GetColor(int i)
{
	return col[i];
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
/*
void hgeFont::SetColor(DWORD col)
{
	dwCol=col;
	for(int i=0;i<256;i++) if(letters[i]) letters[i]->SetColor(col);
}
*/
void hgeFont::SetColor(DWORD _col, int i /*=-1*/)
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

void hgeFont::SetColor(DWORD col0, DWORD col1, DWORD col2, DWORD col3)
{
	col[0] = col0;
	col[1] = col1;
	col[2] = col2;
	col[3] = col3;
	/*
	for(int i=0;i<256;i++) 
		if(letters[i]) 
		{
			letters[i]->SetColor(col0, 0);
			letters[i]->SetColor(col1, 1);
			letters[i]->SetColor(col2, 2);
			letters[i]->SetColor(col3, 3);
		}
	*/
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
void hgeFont::SetZ(float z, hge3DPoint *ptfar)
{
	fZ=z;
	for(int i=0;i<256;i++) if(letters[i]) letters[i]->SetZ(z, z, z, z, ptfar);
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
void hgeFont::SetBlendMode(int blend)
{
	nBlend=blend;
	for(int i=0;i<256;i++) if(letters[i]) letters[i]->SetBlendMode(blend);
}

/************************************************************************/
/* This function is modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
char *hgeFont::_get_line(char *file, char *line)
{
	int i=0;

	if(!file[i]) return 0;

	while(file[i] && file[i]!='\n' && file[i]!='\r')
	{
		line[i]=file[i];
		i++;
	}
	line[i]=0;

	while(file[i] && (file[i]=='\n' || file[i]=='\r')) i++;

	if(file[i]) return file+i;
	else return 0;
}


