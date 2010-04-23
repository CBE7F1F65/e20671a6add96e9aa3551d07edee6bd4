#ifndef WIN32

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>
#include <psprtc.h>
#include "nge_graphics.h"
#include "nge_vram.h"
#include "nge_timer.h"
#include "nge_misc.h"
static unsigned int __attribute__((aligned(16))) list[262144];

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

#define PSP_SLICE_F 0.03125f
#define PSP_PI_DIV_180 0.0174532925
#define MIN(a,b) ((a)<(b)?(a):(b))

static uint32 screen_color = 0;

struct Vertex
{
	unsigned int color;
	float x,y,z;
};

struct VertexUV
{
	float u, v;
	unsigned int color;
	float x,y,z;
};

struct VertexUV_NO_CLOLR
{
	float u, v;
	float x,y,z;
};


static void* m_drawbuf;
static void* m_displaybuf;
static void* m_zbuf;
static ScePspFVector3 m_transmatrix;
static float ProjectionMatrix[16];

//fps count
/**debug show fps*/
u64 m_lasttick;
u64 m_currtick;
uint32 m_tickres;
float m_currms;
uint32 m_fcount;
static uint8 show_fps = 0;
static uint32 m_tex_in_ram = -1;
static uint32 m_tex_id = 0;

static nge_timer* timer = NULL;

static void GetRGBA(int color,int dtype,uint8* r,uint8* g,uint8* b,uint8* a)
{
	switch(dtype)
	{
	case DISPLAY_PIXEL_FORMAT_5551:
		GET_RGBA_5551(color,(*r),(*g),(*b),(*a));
		break;
	case DISPLAY_PIXEL_FORMAT_4444:
		GET_RGBA_4444(color,(*r),(*g),(*b),(*a));
		break;
	case DISPLAY_PIXEL_FORMAT_565:
		GET_RGBA_565(color,(*r),(*g),(*b),(*a));
		break;
	case DISPLAY_PIXEL_FORMAT_8888:
		GET_RGBA_8888(color,(*r),(*g),(*b),(*a));
		break;
	}
}

static void FpsInit()
{
	m_currms = 1.0;
	m_fcount = 0;
	sceRtcGetCurrentTick(&m_lasttick);
	m_tickres = sceRtcGetTickResolution();
}

void ShowFps()
{
	static int init_fps = 0;
	if(init_fps == 0){
		FpsInit();
		init_fps = 1;
	}
	show_fps = 1;	
}

void LimitFps(uint32 limit)
{
	if(limit!=60){
		if(limit == 0)
			limit = 60;
		timer->start(timer);
		if( timer->get_ticks(timer) < 1000 / limit )
		{
				nge_sleep( ( 1000 / limit) - timer->get_ticks(timer) );
		}
	}
	else{
		sceDisplayWaitVblankStart();
	}
}

#include <malloc.h> 
int __freemem() 
{ 
 void *ptrs[480]; 
 int mem, x, i; 
 void *ptr;

 for (x = 0; x < 480; x++) 
 { 
    ptr = malloc(51200); 
    if (!ptr) break; 
  
    ptrs[x] = ptr; 
 } 
 mem = x * 51200; 
 for (i = 0; i < x; i++) 
  free(ptrs[i]); 

 return mem; 
}

static void myShowFps()
{
	static int tick = 10000;
	static int countm = 0;
	static int freemem = 0;
	double time_span=1;
	float curr_fps=1;

	++m_fcount;
	sceRtcGetCurrentTick(&m_currtick);
	{ 
		pspDebugScreenSetOffset((int)m_drawbuf);
		pspDebugScreenSetXY(0,0);
		if (m_currms)
		{
			curr_fps = 1.0f / m_currms;
		}
		if(tick>curr_fps*60){
			tick = 0;
			freemem = __freemem();
			countm++;
		}
		tick++;
		pspDebugScreenPrintf("fps: %.2f \tfreemem: %d bytes(%.2fM) countm=%d",curr_fps,freemem,freemem*1.0/(1024*1024),countm );
		if (m_tickres)
		{
			time_span = ((m_currtick-m_lasttick)) / (float)m_tickres;
		}
		if (m_fcount)
		{
			m_currms = time_span / m_fcount;
		}
		m_fcount = 0;
		sceRtcGetCurrentTick(&m_lasttick);
	}
	m_drawbuf = sceGuSwapBuffers();
}


static void InitGu(void)
{
	// Setup GU
 	//pspDebugScreenInit();
	// Setup GU
 	m_drawbuf = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	m_displaybuf = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	m_zbuf = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);
	// setup GU
	sceGuInit();
	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(GU_PSM_8888,m_drawbuf,BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,m_displaybuf,BUF_WIDTH);
	sceGuDepthBuffer(m_zbuf,BUF_WIDTH);
	sceGuOffset(2048 - (SCREEN_WIDTH/2), 2048 - (SCREEN_HEIGHT/2));
	sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
	// Scissoring
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// Backface culling
	sceGuFrontFace(GU_CCW);
	sceGuDisable(GU_CULL_FACE);		// no culling in 2D
	// Depth test
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);		// disable z-writes
	// Color keying
	sceGuDisable(GU_COLOR_TEST);
	sceGuDisable(GU_ALPHA_TEST);
	sceGuDisable(GU_CLIP_PLANES);
	// Texturing
	sceGuEnable(GU_TEXTURE_2D);
	sceGuShadeModel(GU_SMOOTH);
	sceGuTexWrap(GU_REPEAT, GU_REPEAT);
	sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	//sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexEnvColor(0xFFFFFFFF);
	sceGuColor(0xFFFFFFFF);
	sceGuAmbientColor(0xFFFFFFFF);
	sceGuTexOffset(0.0f, 0.0f);
	sceGuTexScale(1.0f, 1.0f);
	// Blending
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);
	sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	sceGuDisable(GU_DITHER);
	/*if (triBpp<4)
	{
		sceGuSetDither( (ScePspIMatrix4*)DitherMatrix[0] );
		sceGuEnable(GU_DITHER);
	}*/

	
	// Projection
/*
	gumLoadIdentity( (ScePspFMatrix4*)ProjectionMatrix[0] );
	gumOrtho( (ScePspFMatrix4*)ProjectionMatrix[0], 0.0f, 480.0f, 272.0f, 0.0f, -1.0f, 1.0f );
	
	gumLoadIdentity( (ScePspFMatrix4*)ProjectionMatrix[1] );
	ScePspFVector3 displace = { -0.002f, 0.00367f, 0.0f };	// ~ 1/480, 1/272
	gumTranslate( (ScePspFMatrix4*)ProjectionMatrix[1], &displace );
	gumMultMatrix( (ScePspFMatrix4*)ProjectionMatrix[1], (ScePspFMatrix4*)ProjectionMatrix[1], (ScePspFMatrix4*)ProjectionMatrix[0] );

	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadMatrix( (ScePspFMatrix4*)ProjectionMatrix[0] );*/


	ProjectionMatrix[0] = SCREEN_HEIGHT/SCREEN_WIDTH;
	ProjectionMatrix[1] = 0.0f;
	ProjectionMatrix[2] = 0.0f;
	ProjectionMatrix[3] = 0.0f;
	ProjectionMatrix[4] = 0.0f;
	ProjectionMatrix[5] = -1.0f;
	ProjectionMatrix[6] = 0.0f;
	ProjectionMatrix[7] = 0.0f;
	ProjectionMatrix[8] = 0.0f;
	ProjectionMatrix[9] = 0.0f;
	ProjectionMatrix[10] = -1.0f;
	ProjectionMatrix[11] = -1.0f;
	ProjectionMatrix[12] = -SCREEN_HEIGHT/2;
	ProjectionMatrix[13] = SCREEN_HEIGHT/2;
	ProjectionMatrix[14] = SCREEN_HEIGHT/2;
	ProjectionMatrix[15] = SCREEN_HEIGHT/2;

	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumMultMatrix((ScePspFMatrix4*)ProjectionMatrix);

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	
	sceGuClearColor( 0x0 );
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_FAST_CLEAR_BIT);
	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(1);

	// finish
}



void GetVersion()
{
	
}

void InitGrahics()
{
	int i;
	InitGu();
	timer = timer_create();
}
void FiniGrahics()
{
	sceGuTerm();
	timer_free(timer);
}

void SetTexBlend(int src_blend, int des_blend)
{
	//if(src_blend==0&&des_blend==0)
		//glBlendFunc(BLEND_SRC_ALPHA,BLEND_ONE);
	//	sceGuBlendFunc(GU_ADD, BLEND_SRC_ALPHA,BLEND_ONE, 0, 0);
	//else{
		//glBlendFunc(src_blend,des_blend);
		int fixSrc = 0;
		int fixDest = 0;
		if (src_blend == BLEND_ZERO)
			src_blend = GU_FIX;
		else if (src_blend == BLEND_ONE)
		{
			src_blend = GU_FIX;
			fixSrc = 0x00FFFFFF;
		}
		if (des_blend == BLEND_ZERO)
			des_blend = GU_FIX;
		else if (des_blend == BLEND_ONE)
		{
			des_blend = GU_FIX;
			fixDest = 0x00FFFFFF;
		}
		
		//glBlendFunc(src, dest);
		sceGuBlendFunc(GU_ADD, src_blend, des_blend, fixSrc, fixDest);
	//}
}

void ResetTexBlend()
{
	//glBlendFunc(BLEND_SRC_ALPHA,BLEND_ONE_MINUS_SRC_ALPHA);
	sceGuBlendFunc(GU_ADD, BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA, 0, 0);
}

void BeginScene(uint8 clear)
{
	sceGuStart(GU_DIRECT,list);
	if(clear){
		sceGuDisable(GU_SCISSOR_TEST);
		sceGuClearColor(screen_color);
		sceGuClear(GU_COLOR_BUFFER_BIT);
		sceGuEnable(GU_SCISSOR_TEST);
	}

	ProjectionMatrix[0] = SCREEN_HEIGHT/SCREEN_WIDTH;
	ProjectionMatrix[1] = 0.0f;
	ProjectionMatrix[2] = 0.0f;
	ProjectionMatrix[3] = 0.0f;
	ProjectionMatrix[4] = 0.0f;
	ProjectionMatrix[5] = -1.0f;
	ProjectionMatrix[6] = 0.0f;
	ProjectionMatrix[7] = 0.0f;
	ProjectionMatrix[8] = 0.0f;
	ProjectionMatrix[9] = 0.0f;
	ProjectionMatrix[10] = -1.0f;
	ProjectionMatrix[11] = -1.0f;
	ProjectionMatrix[12] = -SCREEN_HEIGHT/2;
	ProjectionMatrix[13] = SCREEN_HEIGHT/2;
	ProjectionMatrix[14] = SCREEN_HEIGHT/2;
	ProjectionMatrix[15] = SCREEN_HEIGHT/2;

	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumMultMatrix((ScePspFMatrix4*)ProjectionMatrix);

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
}

uint32 SetScreenColor(uint8 r,uint8 g,uint8 b,uint8 a)
{
	uint32 u_lastcolor;
	u_lastcolor = screen_color;
	screen_color = MAKE_RGBA_8888(r,g,b,a);
	return u_lastcolor;
}

void EndScene()
{
	sceGuFinish();
	sceGuSync(0,0);
	if(show_fps == 1){
		myShowFps();
	}
	else
		sceGuSwapBuffers();
}

void DrawLine(float x1, float y1, float x2, float y2, int color,int dtype)
{
	struct Vertex* vertices = (struct Vertex*)sceGuGetMemory(2 * sizeof(struct Vertex));
	if (!vertices)
	{
		return;
	}
	vertices[0].color = color;
	vertices[0].x = x1; 
	vertices[0].y = y1; 
	vertices[0].z = 0.0f;

	vertices[1].color = color;
	vertices[1].x = x2; 
	vertices[1].y = y2; 
	vertices[1].z = 0.0f;
	
	sceGuDisable(GU_TEXTURE_2D);
	sceGuShadeModel(GU_FLAT);
	sceGuShadeModel(GU_SMOOTH);
	sceGuAmbientColor(0xffffffff);
	sceGumDrawArray(GU_LINES, dtype|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 1*2, 0, vertices);
	sceGuEnable(GU_TEXTURE_2D);
}

void DrawLineEx(pointf p1,pointf p2 ,int color,int dtype)
{
	DrawLine(p1.x, p1.y, p2.x, p2.y,color,dtype);
}

void SetClip(int x,int y,int w,int h)
{
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(x,y,x+w,y+h);
}

void ResetClip()
{
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void RenderHGEQuad(const hgeQuad * quad, image_p texture)
{
	struct VertexUV *vertices;
	int i;

	if(texture == 0 || texture->w == 0 || texture->h ==0 || texture->texw == 0 || texture->texh == 0)
		return ;
/*

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();*/


	if(texture->swizzle == 0 && texture->dontswizzle ==0){
		swizzle_swap(texture);
	}
	sceGuTexMode(texture->mode,0,0,texture->swizzle);
	sceKernelDcacheWritebackAll();
	if((texture->modified==1)||texture->texid != m_tex_in_ram){
		m_tex_in_ram = texture->texid;
		texture->modified = 0;
		sceGuTexImage(0,texture->texw,texture->texh,texture->texw,texture->data);
		sceKernelDcacheWritebackAll();
	}

	vertices = (struct VertexUV*)sceGuGetMemory(4 * sizeof(struct VertexUV));
	if (!vertices)
	{
		return;
	}
	for (i=0; i<4; i++)
	{
		int j = i;
		uint8 r, g, b, a;
		if (i > 1)
		{
			j = 5-i;
		}
		vertices[i].u = quad->v[j].tx;
		vertices[i].v = quad->v[j].ty;
		vertices[i].x = quad->v[j].x;
		vertices[i].y = quad->v[j].y;
		vertices[i].z = quad->v[j].z;
//		GetRGBA(quad->v[j].col, texture->dtype, &b, &g, &r, &a);
		vertices[i].color = quad->v[j].col;//MAKE_RGBA_8888(b, a, g, r);
	}
	sceGumDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|(texture->dtype)|GU_VERTEX_32BITF|GU_TRANSFORM_3D|GU_COLOR_8888,4,0,vertices);
}

image_p ScreenToImage()
{
	void* temp;
	int width,format,dtype;
	image_p pimage = NULL;
	int i,x,y;
	uint8 r,g,b;
	uint32 color;
	uint32 *vram32;
	uint16 *vram16;
	uint8 *line;
	sceDisplayWaitVblankStart();  // if framebuf was set with PSP_DISPLAY_SETBUF_NEXTFRAME, wait until it is changed
    sceDisplayGetFrameBuf(&temp, &width, &format, PSP_DISPLAY_SETBUF_NEXTFRAME);
	pimage = image_create(SCREEN_WIDTH,SCREEN_HEIGHT,DISPLAY_PIXEL_FORMAT_8888, 0);
	if(pimage == NULL)
		return NULL;
	vram32 = (uint32*) temp;
    vram16 = (uint16*) vram32;
	line = (uint8*)pimage->data;
    for (y = 0; y < SCREEN_HEIGHT; y++) {
                for (i = 0, x = 0; x < SCREEN_WIDTH; x++) {
                       switch (format) {
                                case PSP_DISPLAY_PIXEL_FORMAT_565:
                                        color = vram16[x + y * width];
                                        r = (color & 0x1f) << 3; 
                                        g = ((color >> 5) & 0x3f) << 2 ;
                                        b = ((color >> 11) & 0x1f) << 3 ;
                                        break;
                                case PSP_DISPLAY_PIXEL_FORMAT_5551:
                                        color = vram16[x + y * width];
                                        r = (color & 0x1f) << 3; 
                                        g = ((color >> 5) & 0x1f) << 3 ;
                                        b = ((color >> 10) & 0x1f) << 3 ;
                                        break;
                                case PSP_DISPLAY_PIXEL_FORMAT_4444:
                                        color = vram16[x + y * width];
                                        r = (color & 0xf) << 4; 
                                        g = ((color >> 4) & 0xf) << 4 ;
                                        b = ((color >> 8) & 0xf) << 4 ;
                                        break;
                                case PSP_DISPLAY_PIXEL_FORMAT_8888:
                                        color = vram32[x + y * width];
                                        r = color & 0xff; 
                                        g = (color >> 8) & 0xff;
                                        b = (color >> 16) & 0xff;
                                        break;
                        }
                        line[i++] = r;
                        line[i++] = g;
                        line[i++] = b;
						line[i++] = 0xff;
                }
                line = line+pimage->texw*pimage->bpb;
        }
	return pimage;
}


void ScreenShot(const char* filename)
{
	image_p pimage = ScreenToImage();
	if(filename == NULL||pimage == NULL)
		return;
	image_save_png(pimage,filename,1);
	image_free(pimage);
}


#endif
