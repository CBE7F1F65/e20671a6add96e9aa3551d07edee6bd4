#ifndef _HGEDEFINES_H
#define _HGEDEFINES_H

#ifndef ZeroMemory
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#ifndef WIN32
#include <psptypes.h>
#endif

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

#ifndef DWORD
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
#endif

#define HGETEXT_LEFT		0
#define HGETEXT_RIGHT		1
#define HGETEXT_CENTER		2
#define HGETEXT_HORZMASK	0x03

#define HGETEXT_TOP			0
#define HGETEXT_BOTTOM		4
#define HGETEXT_MIDDLE		8
#define HGETEXT_VERTMASK	0x0C

/************************************************************************/
/* This define is added by h5nc (h5nc@yahoo.com.cn)                     */
/************************************************************************/
#ifndef QWORD
#ifdef WIN32
typedef unsigned __int64	QWORD;
#else
typedef u64	QWORD;
#endif
#endif

#ifndef LONGLONG
#ifdef WIN32
typedef __int64 LONGLONG;
#else
typedef s64 LONGLONG;
#endif
#endif

#ifndef NULL
#define NULL	0
#endif

#define M_PATHMAX		_MAX_PATH
#define M_STRMAX		0x80
#define M_STRITOAMAX	0x10

/*
** Common math constants
*/
#ifndef M_PI
#define M_PI	3.14159265358979323846f
#define M_PI_2	1.57079632679489661923f
#define M_PI_4	0.785398163397448309616f
#define M_1_PI	0.318309886183790671538f
#define M_2_PI	0.636619772367581343076f
#endif

/************************************************************************/
/* These defines are added by h5nc (h5nc@yahoo.com.cn)                  */
/************************************************************************/
#ifndef M_E
#define M_E        2.71828182845904523536f
#define M_LOG2E    1.44269504088896340736f
#define M_LOG10E   0.434294481903251827651f
#define M_LN2      0.693147180559945309417f
#define M_LN10     2.30258509299404568402f
#define M_2_SQRTPI 1.12837916709551257390f
#define M_SQRT2    1.41421356237309504880f
#define M_SQRT1_2  0.707106781186547524401f
#endif

/************************************************************************/
/* This define is added by h5nc (h5nc@yahoo.com.cn)                     */
/************************************************************************/
// Texture Offset
#define HGE_TEXTUREOFFSET	0.0f

/*
** HGE Handle types
*/
typedef DWORD HTEXTURE;
typedef DWORD HTARGET;
typedef DWORD HEFFECT;
typedef DWORD HMUSIC;
typedef DWORD HSTREAM;
typedef DWORD HCHANNEL;

/************************************************************************/
/* This define is added by Yuki                                         */
/* h5nc (h5nc@yahoo.com.cn) copied her codes with her permission        */
/************************************************************************/
/* add by Yuki */
typedef DWORD HD3DFONT;

/*
** Hardware color macros
*/
#define ARGB(a,r,g,b)	((DWORD(a)<<24) + (DWORD(r)<<16) + (DWORD(g)<<8) + DWORD(b))
#define GETA(col)		((col)>>24)
#define GETR(col)		(((col)>>16) & 0xFF)
#define GETG(col)		(((col)>>8) & 0xFF)
#define GETB(col)		((col) & 0xFF)
#define SETA(col,a)		(((col) & 0x00FFFFFF) + (DWORD(a)<<24))
#define SETR(col,r)		(((col) & 0xFF00FFFF) + (DWORD(r)<<16))
#define SETG(col,g)		(((col) & 0xFFFF00FF) + (DWORD(g)<<8))
#define SETB(col,b)		(((col) & 0xFFFFFF00) + DWORD(b))


/*
** HGE Blending constants
*/
#define	BLEND_COLORADD		1
#define	BLEND_COLORMUL		0
#define	BLEND_ALPHABLEND	2
#define	BLEND_ALPHAADD		0
#define	BLEND_ZWRITE		4
#define	BLEND_NOZWRITE		0

#define BLEND_DEFAULT		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE)
#define BLEND_DEFAULT_Z		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)


/*
** HGE System state constants
*/

/************************************************************************/
/* This defines are added by h5nc (h5nc@yahoo.com.cn)                   */
/************************************************************************/
#define HGE_STATE_MASK		0xff00
#define HGE_STATE_BOOLTEST		0x0100
#define HGE_STATE_FUNCTEST		0x0200
#define HGE_STATE_HWNDTEST		0x0300
#define HGE_STATE_INTTEST		0x0400
#define HGE_STATE_STRINGTEST	0x0500


/************************************************************************/
/* Thes defines are modified by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
enum hgeBoolState
{
	HGE_WINDOWED		= 0x0101,    // bool		run in window?		(default: false)
	HGE_ZBUFFER			= 0x0102,    // bool		use z-buffer?		(default: false)
	HGE_TEXTUREFILTER	= 0x0103,    // bool		texture filtering?	(default: true)
	
	HGE_USESOUND		= 0x0104,    // bool		use BASS for sound?	(default: true)
	
	HGE_DONTSUSPEND		= 0x0105,	// bool		focus lost:suspend?	(default: false)
	HGE_HIDEMOUSE		= 0x0106,	// bool		hide system cursor?	(default: true)
	
	HGE_2DMODE			= 0x0107,	// bool		2d mode? (default: true)
	
	HGEBOOLSTATE_FORCE_DWORD = 0x7FFFFFFF
};

enum hgeFuncState
{
	HGE_FRAMEFUNC		= 0x0201,    // bool*()	frame function		(default: NULL) (you MUST set this)
	HGE_RENDERFUNC		= 0x0202,    // bool*()	render function		(default: NULL)
	HGE_FOCUSLOSTFUNC	= 0x0203,   // bool*()	focus lost function	(default: NULL)
	HGE_FOCUSGAINFUNC	= 0x0204,   // bool*()	focus gain function	(default: NULL)
	HGE_GFXRESTOREFUNC	= 0x0205,   // bool*()	graphics restore function		(default: NULL)
	HGE_EXITFUNC		= 0x0206,   // bool*()	exit function		(default: NULL)
	
	HGEFUNCSTATE_FORCE_DWORD = 0x7FFFFFFF
};

enum hgeHwndState
{
	HGE_HWND			= 0x0301,	// int		window handle: read only
	HGE_HWNDPARENT		= 0x0302,	// int		parent win handle	(default: 0)
	
	HGEHWNDSTATE_FORCE_DWORD = 0x7FFFFFFF
};

enum hgeIntState
{
	HGE_SCREENWIDTH		= 0x0401,   // int		screen width		(default: 800)
	HGE_SCREENHEIGHT	= 0x0402,   // int		screen height		(default: 600)
	HGE_SCREENBPP		= 0x0403,   // int		screen bitdepth		(default: 32) (desktop bpp in windowed mode)
	
	HGE_SAMPLERATE		= 0x0404,   // int		sample rate			(default: 44100)
	HGE_SAMPLEVOLUME	= 0x0405,	// int		global sample volume(default: 10000)
	HGE_FXVOLUME		= 0x0406,   // int		global fx volume	(default: 10000)
//	HGE_MUSVOLUME		= 0x0406,   // int		global music volume	(default: 10000)
	HGE_STREAMVOLUME	= 0x0407,   // int		global music volume	(default: 10000)	
	HGE_FPS				= 0x0408,	// int		fixed fps			(default: HGEFPS_UNLIMITED)

	/************************************************************************/
	/* These defines are added by h5nc (h5nc@yahoo.com.cn)                  */
	/************************************************************************/
	//Begin
	HGE_FRAMECOUNTER	= 0x0409,	//int		frame counter;	read only
	HGE_FRAMESKIP		= 0x040A,	//int		frame skip			(default: 0)
	HGE_RENDERSKIP		= 0x040B,	//int		render skip			(default: 0)
	//End
	
	HGEINTSTATE_FORCE_DWORD = 0x7FFFFFF
};

enum hgeStringState
{
	HGE_ICON			= 0x0501,   // char*	icon resource		(default: NULL)
	HGE_TITLE			= 0x0502,   // char*	window title		(default: "HGE")
	
	HGE_INIFILE			= 0x0503,   // char*	ini file			(default: NULL) (meaning no file)
	HGE_LOGFILE			= 0x0504,   // char*	log file			(default: NULL) (meaning no file)

	/************************************************************************/
	/* This define is added by h5nc (h5nc@yahoo.com.cn)                     */
	/************************************************************************/
	HGE_BASSDLLFILE		= 0x0505,	// char*	bass.dll file		(default: "BASS.dll")

	HGESTRINGSTATE_FORCE_DWORD = 0x7FFFFFFF
};

/*
** HGE_FPS system state special constants
*/
#define HGEFPS_UNLIMITED	0
#define HGEFPS_VSYNC		-1
//
/************************************************************************/
/* This define is added by h5nc (h5nc@yahoo.com.cn)                     */
/************************************************************************/
#define HGEFPS_MAX			1000

/*
** HGE Primitive type constants
*/
#define HGEPRIM_LINES		2
#define HGEPRIM_TRIPLES		3
#define HGEPRIM_QUADS		4


/*
** HGE Vertex structure
*/
typedef struct tagHgeVertex
{
	float			x, y;		// screen position    
	float			z;			// Z-buffer depth 0..1
	DWORD			col;		// color
	float			tx, ty;		// texture coordinates
}hgeVertex;


/*
** HGE Triple structure
*/
typedef struct tagHgeTriple
{
	hgeVertex		v[3];
	HTEXTURE		tex;
	int				blend;
}hgeTriple;


/*
** HGE Quad structure
*/
typedef struct tagHgeQuad
{
	hgeVertex		v[4];
	HTEXTURE		tex;
	int				blend;
}hgeQuad;


/*
** HGE Input Event structure
*/
typedef struct tagHgeInputEvent
{
	int		type;			// event type
	int		key;			// key code
	int		flags;			// event flags
	int		chr;			// character code
	int		wheel;			// wheel shift
	float	x;				// mouse cursor x-coordinate
	float	y;				// mouse cursor y-coordinate
}hgeInputEvent;

#ifndef CALL
#ifdef WIN32
#define CALL  __stdcall
#else
#define CALL
#endif
#endif
/*
** HGE Input Event type constants
*/
#define INPUT_KEYDOWN		1
#define INPUT_KEYUP			2
#define INPUT_MBUTTONDOWN	3
#define INPUT_MBUTTONUP		4
#define INPUT_MOUSEMOVE		5
#define INPUT_MOUSEWHEEL	6


/*
** HGE Input Event flags
*/
#define HGEINP_SHIFT		1
#define HGEINP_CTRL			2
#define HGEINP_ALT			4
#define HGEINP_CAPSLOCK		8
#define HGEINP_SCROLLLOCK	16
#define HGEINP_NUMLOCK		32
#define HGEINP_REPEAT		64

/************************************************************************/
/* This define is added by h5nc (h5nc@yahoo.com.cn)                     */
/************************************************************************/
/*
**HGE Memory File Info
*/
struct hgeMemoryFile
{
	char * filename;
	BYTE * data;
	DWORD size;
};

/************************************************************************/
/* This define is added by h5nc (h5nc@yahoo.com.cn)                     */
/************************************************************************/
/*
**HGE Channel Sync Info
*/

struct hgeChannelSyncInfo 
{
	QWORD startPos;
	QWORD allLength;
	QWORD introLength;
	DWORD/*HSYNC*/ sync;
};

/************************************************************************/
/* These defines are added by h5nc (h5nc@yahoo.com.cn)                  */
/************************************************************************/
/*
**HGE Point
*/

struct hge2DPoint 
{
	float x;
	float y;
};

struct hge3DPoint 
{
	float x;
	float y;
	union {
		float z;
		float scale;
	};
};

/************************************************************************/
/* These defines are added by h5nc (h5nc@yahoo.com.cn)                  */
/************************************************************************/
// begin
#define DIKEY_PRESSED	0x0
#define DIKEY_UP		0x10
#define DIKEY_DOWN		0x20

#define JOY_LEFT		0x40
#define JOY_RIGHT		0x50
#define JOY_UP			0x60
#define JOY_DOWN		0x70

#define DIJOY_MAXDEVICE	4
// end

/*
** HGE Virtual-key codes
*/
#define HGEK_LBUTTON	0x01
#define HGEK_RBUTTON	0x02
#define HGEK_MBUTTON	0x04

#define HGEK_ESCAPE		0x1B
#define HGEK_BACKSPACE	0x08
#define HGEK_TAB		0x09
#define HGEK_ENTER		0x0D
#define HGEK_SPACE		0x20

#define HGEK_SHIFT		0x10
#define HGEK_CTRL		0x11
#define HGEK_ALT		0x12

#define HGEK_LWIN		0x5B
#define HGEK_RWIN		0x5C
#define HGEK_APPS		0x5D

#define HGEK_PAUSE		0x13
#define HGEK_CAPSLOCK	0x14
#define HGEK_NUMLOCK	0x90
#define HGEK_SCROLLLOCK	0x91

#define HGEK_PGUP		0x21
#define HGEK_PGDN		0x22
#define HGEK_HOME		0x24
#define HGEK_END		0x23
#define HGEK_INSERT		0x2D
#define HGEK_DELETE		0x2E

#define HGEK_LEFT		0x25
#define HGEK_UP			0x26
#define HGEK_RIGHT		0x27
#define HGEK_DOWN		0x28

#define HGEK_0			0x30
#define HGEK_1			0x31
#define HGEK_2			0x32
#define HGEK_3			0x33
#define HGEK_4			0x34
#define HGEK_5			0x35
#define HGEK_6			0x36
#define HGEK_7			0x37
#define HGEK_8			0x38
#define HGEK_9			0x39

#define HGEK_A			0x41
#define HGEK_B			0x42
#define HGEK_C			0x43
#define HGEK_D			0x44
#define HGEK_E			0x45
#define HGEK_F			0x46
#define HGEK_G			0x47
#define HGEK_H			0x48
#define HGEK_I			0x49
#define HGEK_J			0x4A
#define HGEK_K			0x4B
#define HGEK_L			0x4C
#define HGEK_M			0x4D
#define HGEK_N			0x4E
#define HGEK_O			0x4F
#define HGEK_P			0x50
#define HGEK_Q			0x51
#define HGEK_R			0x52
#define HGEK_S			0x53
#define HGEK_T			0x54
#define HGEK_U			0x55
#define HGEK_V			0x56
#define HGEK_W			0x57
#define HGEK_X			0x58
#define HGEK_Y			0x59
#define HGEK_Z			0x5A

#define HGEK_GRAVE		0xC0
#define HGEK_MINUS		0xBD
#define HGEK_EQUALS		0xBB
#define HGEK_BACKSLASH	0xDC
#define HGEK_LBRACKET	0xDB
#define HGEK_RBRACKET	0xDD
#define HGEK_SEMICOLON	0xBA
#define HGEK_APOSTROPHE	0xDE
#define HGEK_COMMA		0xBC
#define HGEK_PERIOD		0xBE
#define HGEK_SLASH		0xBF

#define HGEK_NUMPAD0	0x60
#define HGEK_NUMPAD1	0x61
#define HGEK_NUMPAD2	0x62
#define HGEK_NUMPAD3	0x63
#define HGEK_NUMPAD4	0x64
#define HGEK_NUMPAD5	0x65
#define HGEK_NUMPAD6	0x66
#define HGEK_NUMPAD7	0x67
#define HGEK_NUMPAD8	0x68
#define HGEK_NUMPAD9	0x69

#define HGEK_MULTIPLY	0x6A
#define HGEK_DIVIDE		0x6F
#define HGEK_ADD		0x6B
#define HGEK_SUBTRACT	0x6D
#define HGEK_DECIMAL	0x6E

#define HGEK_F1			0x70
#define HGEK_F2			0x71
#define HGEK_F3			0x72
#define HGEK_F4			0x73
#define HGEK_F5			0x74
#define HGEK_F6			0x75
#define HGEK_F7			0x76
#define HGEK_F8			0x77
#define HGEK_F9			0x78
#define HGEK_F10		0x79
#define HGEK_F11		0x7A
#define HGEK_F12		0x7B

typedef struct tagCTextureList
{
	HTEXTURE			tex;
	int					width;
	int					height;
	struct tagCTextureList*		next;
}CTextureList;

typedef struct tagCResourceList
{
	char				filename[_MAX_PATH];
	int					password;
	struct tagCResourceList*		next;
}CResourceList;

typedef struct tagCStreamList
{
	HSTREAM				hstream;
	void*				data;
	struct tagCStreamList*		next;
}CStreamList;

typedef struct tagCInputEventList
{
	hgeInputEvent		event;
	struct tagCInputEventList*	next;
}CInputEventList;

typedef struct tagCFontList
{
	HD3DFONT font;
	struct tagCFontList * next;
}CFontList;


#endif