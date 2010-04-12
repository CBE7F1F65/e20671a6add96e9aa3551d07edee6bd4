
#include <vector>
#include "../../src/libnge2.h"
#include "../../src/hge/HGEExport.h"
/**
 * nge_test:��򵥵�nge����:��ʾһ��ͼƬ
 */

//�˳���ʶ
int game_quit = 0;
//����ͼƬ
image_p p_bg = NULL;
//logoͼƬ
image_p p_logo  = NULL;
//logoͼƬ��ͼƬ����
int logomask1,logomask2;

void btn_down(int keycode)
{
	switch(keycode)
    {
	case PSP_BUTTON_UP:
		break;
	case PSP_BUTTON_DOWN:
		break;
	case PSP_BUTTON_LEFT:
		break;
	case PSP_BUTTON_RIGHT: 
		break;
	case PSP_BUTTON_TRIANGLE:
		break;
	case PSP_BUTTON_CIRCLE:
		break;
	case PSP_BUTTON_CROSS:
		break;
    case PSP_BUTTON_SQUARE:
		break;
	case PSP_BUTTON_SELECT:
		//����ѡ����˳�
		game_quit = 1;
		break;
	case PSP_BUTTON_START:
		//���¿�ʼ���˳�
		game_quit = 1;
		break;
    }
}


hgeQuad quad;
void DrawScene()
{

	BeginScene(1);
	ImageToScreen(p_bg,0,0);
	if (quad.tex)
	{;
		hge->Gfx_RenderQuad(&quad);
	}
/*
	DrawImage(p_logo,0,0,0,0,128,0,256,256);
	DrawImageMask(p_logo,0,0,0,0,0/ *dx* /,0/ *dy* /,128/ *dw* /,128/ *dh* /,logomask1);
	DrawImageMask(p_logo,0,0,0,0,480-128/ *dx* /,272-128/ *dy* /,128/ *dw* /,128/ *dh* /,0x7fffffff);
	DrawImageMask(p_logo,0,0,0,0,0/ *dx* /,272-128/ *dy* /,128/ *dw* /,128/ *dh* /,logomask2);
	DrawImageMask(p_logo,0,0,0,0,480-128/ *dx* /,0/ *dy* /,128/ *dw* /,128/ *dh* /,logomask2);*/

	EndScene();
}

extern "C"
int main(int argc, char* argv[])
{
	//��ʼ��NGE��ΪVIDEO,AUDIO��������ֻ��ʼ��VIDEO�������ʼ��������INIT_VIDEO|INIT_AUDIO,����INIT_ALL
	NGE_Init(INIT_VIDEO);
	//��ʼ����������btn_down�ǰ�����Ӧ,�����ǵ���ʱ����Ӧ��0����nge����home��Ϣ(ֱ���˳�),��1������PSPϵͳ����
	//home��Ϣ,ͨ����1�����˳���1.50������Ƴ�����Ҫ��0��
	std::vector<int> v;
	//���һ��������psp swizzle�Ż���ͨ����1
	p_bg = image_load("images/demo0.jpg",DISPLAY_PIXEL_FORMAT_8888,1);

	quad.tex = (HTEXTURE)p_bg;
	quad.blend = BLEND_DEFAULT;
	quad.v[0].tx = 0;	quad.v[0].ty = 0;	quad.v[0].col = 0xFFFFFFFF;
	quad.v[0].x = 0;	quad.v[0].y = 0;	quad.v[0].z = 0;
	quad.v[1].tx = 1;	quad.v[1].ty = 0;	quad.v[1].col = 0xFFFFFFFF;
	quad.v[1].x = SCREEN_WIDTH;	quad.v[1].y = 0;	quad.v[1].z = 0;
	quad.v[2].tx = 1;	quad.v[2].ty = 1;	quad.v[2].col = 0xFFFFFFFF;
	quad.v[2].x = SCREEN_WIDTH;	quad.v[2].y = SCREEN_HEIGHT;	quad.v[2].z = 0;
	quad.v[3].tx = 0;	quad.v[3].ty = 1;	quad.v[3].col = 0xFFFFFFFF;
	quad.v[3].x = 0;	quad.v[3].y = SCREEN_HEIGHT;	quad.v[3].z = 0;

	if(p_bg == NULL)
		printf("can not open file\n");
	p_logo = image_load("images/nge2logo.png",DISPLAY_PIXEL_FORMAT_4444,1);
	if(p_logo == NULL)
		printf("can not open file\n");
	//����һ����͸����ͼƬ����color
	logomask1 = CreateColor(255,255,255,128,p_logo->dtype);
	//��㴴��һ��ͼƬ����color
	logomask2 = CreateColor(100,100,100,255,p_logo->dtype);
	
	while ( !game_quit )
	{
		ShowFps();
		InputProc();
		DrawScene();
	}
	image_free(p_bg);
	image_free(p_logo);
	NGE_Quit();
	return 0;
}



