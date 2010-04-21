#include <gl/glut.h>
#include <GL/gl.h>
#include "../../win32depends/include/SDL.h"
#include <math.h>
#include "nge_graphics.h"
#include "nge_image_load.h"
#include "nge_timer.h"
#include "nge_misc.h"
//fps count
static uint32 m_frame = 0;
static uint32 m_t0 = 0;

static uint32 m_tex_in_ram = -1;
static uint32 m_tex_id = 0;
static nge_timer* timer = NULL;

static float screen_r =0.0;
static float screen_g =0.0;
static float screen_b =0.0;
static float screen_a =1.0;

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

void GetVersion()
{
	int main_version, sub_version, release_version;
	const char* version = (const char*)glGetString(GL_VERSION);
	sscanf(version, "%d.%d.%d", &main_version, &sub_version, &release_version);
	printf("OpenGL 版本：%s\n", version);
	printf("主版本号：%d\n", main_version);
	printf("次版本号：%d\n", sub_version);
	printf("发行版本号：%d\n", release_version);
}

void SetTexBlend(int src_blend, int des_blend)
{
	if(src_blend==0&&des_blend==0)
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	else
		glBlendFunc(src_blend,des_blend);
}
void ResetTexBlend()
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void InitGrahics()
{
	timer = timer_create();
	
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glGenTextures( 1, &m_tex_id );
	glBindTexture(GL_TEXTURE_2D, m_tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_SCISSOR_TEST);
	ResetClip();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glScalef(1, -1, 1);
	gluPerspective(90, SCREEN_WIDTH/SCREEN_HEIGHT, 0, SCREEN_HEIGHT/2);
	glTranslatef(-SCREEN_WIDTH/2, -SCREEN_HEIGHT/2, -SCREEN_HEIGHT/2);
}
void FiniGrahics()
{
	timer_free(timer);
}

void BeginScene(uint8 clear)
{
	if(clear == 1){
		glDisable(GL_SCISSOR_TEST);
		glClearColor( screen_r, screen_g, screen_b, screen_a );
		glClear( GL_COLOR_BUFFER_BIT );
		glEnable(GL_SCISSOR_TEST);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScalef(1, -1, 1);
	gluPerspective(90, SCREEN_WIDTH/SCREEN_HEIGHT, 0, SCREEN_HEIGHT/2);
	glTranslatef(-SCREEN_WIDTH/2, -SCREEN_HEIGHT/2, -SCREEN_HEIGHT/2);
}

uint32 SetScreenColor(uint8 r,uint8 g,uint8 b,uint8 a)
{
	uint32 u_lastcolor;
	u_lastcolor = MAKE_RGBA_8888(((int)(screen_r*255)),((int)(screen_g*255)),((int)(screen_b*255)),((int)(screen_a*255)));
	screen_r = r/255.0;
	screen_g = g/255.0;
	screen_b = b/255.0;
	screen_a = a/255.0;
	return u_lastcolor;
}


void EndScene()
{
	SDL_GL_SwapBuffers();
}

void FpsInit()
{
	m_frame = 0;
	m_t0 = 0;
}

void SetClip(int x,int y,int w,int h)
{
	glScissor(x,SCREEN_HEIGHT-y-h,w,h);
}

void ResetClip()
{
	SetClip(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void ShowFps()
{
	static int init_fps = 0;
	int t;
	float seconds,fps;

	if(init_fps == 0){
		FpsInit();
		init_fps = 1;
	}
	m_frame++;
   	t = SDL_GetTicks();
	if (t - m_t0 >= 1000) {
			seconds = (t - m_t0) / 1000.0;
			fps = m_frame / seconds;
			printf("%d frames in %g seconds = %g FPS\n", m_frame, seconds, fps);
			m_t0 = t;
			m_frame = 0;
	}   
}

void LimitFps(uint32 limit)
{
	if(limit == 0)
		limit = 60;
	timer->start(timer);
	if( timer->get_ticks(timer) < 1000 /limit )
    {
		nge_sleep( ( 1000 / limit) - timer->get_ticks(timer) );
    }
}

void DrawLine(float x1, float y1, float x2, float y2, int color,int dtype)
{
	static uint8 r,g,b,a;
	
	if(y1 == 0.0)
		y1 = 0.1;
	if(y2 == 0.0)
		y2 = 0.1;
	glDisable(GL_TEXTURE_2D);
	glPushAttrib(GL_CURRENT_BIT);
	GetRGBA(color,dtype,&r,&g,&b,&a);
	glColor4ub(r, g, b, a);
	glBegin(GL_LINES);
		glVertex2f(x1, y1);	
		glVertex2f(x2, y2);
	glEnd();
	glPopAttrib();
	glEnable(GL_TEXTURE_2D);
}

void DrawLineEx(pointf p1,pointf p2 ,int color,int dtype)
{
	DrawLine(p1.x, p1.y, p2.x, p2.y,color,dtype);
}

int PreLoadImage(image_p pimg)
{
	return 1;
}

int TexImage2D(image_p pimg)
{
	uint32 format = GL_RGBA;
	m_tex_in_ram = pimg->texid;
	if(pimg->dtype == DISPLAY_PIXEL_FORMAT_565){
			  format = GL_RGB;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, 4, pimg->texw, pimg->texh, 0, format, pimg->dtype, pimg->data);
	return 1;
}

void RenderHGEQuad(const hgeQuad * quad)
{
	image_p texture = (image_p)quad->tex;
	int i;
	uint8 r, g, b, a;
	if((texture->modified==1)||texture->texid != m_tex_in_ram){
		TexImage2D(texture);
		texture->modified = 0;
	}
	glPushAttrib(GL_CURRENT_BIT);
	glBegin(GL_QUADS);
	for (i=3; i>=0; i--)
	{
		GetRGBA(quad->v[i].col, texture->dtype, &b, &g, &r, &a);
		glColor4ub(r, g, b, a);
		glTexCoord2f(quad->v[i].tx, quad->v[i].ty); glVertex3f(quad->v[i].x, quad->v[i].y, quad->v[i].z);
	}
	glEnd();
	glPopAttrib();
}


image_p ScreenToImage()
{
	image_p pimage = image_create(SCREEN_WIDTH,SCREEN_HEIGHT,DISPLAY_PIXEL_FORMAT_8888);
	if(pimage == NULL)
		return NULL;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glReadPixels( 0, 0, TEXTRUE_MAX_WIDTH, TEXTRUE_MAX_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pimage->data );
	image_flipv(pimage);
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

