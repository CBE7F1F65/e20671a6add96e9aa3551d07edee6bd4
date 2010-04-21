#ifndef NGE_GRAPHICS_H_
#define NGE_GRAPHICS_H_
#include "nge_define.h"
#include "hge/hgeDefines.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * ��ʼ������,��nge2ϵͳ����,�û�������ʽ����
 */
void InitGrahics();
/**
 * ��������,��nge2ϵͳ����,�û�������ʽ����
 */
void FiniGrahics();
/**
 * ��Ļ���ƿ�ʼ����,���л��Ƶ���Ļ����BeginScene,EndScene֮�����
 *@param uint8 clear,������־��Ĭ��Ϊ1����
 *@return ��
 */
void BeginScene(uint8 clear);
/**
 * ��Ļ���ƽ�������,���л��Ƶ���Ļ����BeginScene,EndScene֮�����
 *@return ��
 */
void EndScene();
/**
 * �����Ƿ���ʾFPS�͵�����Ϣ
 *@return ��
 */
void ShowFps();
/**
 * ����֡��
 *@param int fps,Ŀ��֡��
 *@return ��
 */
void LimitFps(uint32 fps);
/**
 * ���òü�������Ŀ�������ڵĲ���ʾ��ͬJ2ME��SetClip
 *@param int x,Ŀ������x����
 *@param int y,Ŀ������y����
 *@param int w,Ŀ������ü���width
 *@param int h,Ŀ������ü���height
 *@return ��
 */
void SetClip(int x,int y,int w,int h);
/**
 * �����òü����򣬵�ͬ�ڵ���SetClip(0,0,SCREEN_WIDTH,SCREEN_HEIGHT)
 *@return ��
 */
void ResetClip();
/**
 * ���ߺ���1�����������
 *@param float x1,��һ����x����
 *@param float y1,��һ����y����
 *@param float x2,�ڶ�����x����
 *@param float y2,�ڶ�����y����
 *@param int color,��ɫ�������Ӧ�����������õ�5551���������MAKE_RGBA5551
 *@param int dtype,��ʾģʽ��������Ķ�Ӧ
 *@return ��
 */
void DrawLine(float x1, float y1, float x2, float y2, int color,int dtype);
/**
 * ���ߺ���2�����������pointf
 *@param pointf p1,��һ����xy����
 *@param pointf p2,�ڶ�����xy����
 *@param int color,��ɫ�������Ӧ�����������õ�5551���������MAKE_RGBA5551
 *@param int dtype,��ʾģʽ��������Ķ�Ӧ
 *@return ��
 */
void DrawLineEx(pointf p1,pointf p2, int color,int dtype);
/**
 * ��ͼ����1��Ч��������ImageToScreen>DrawImage>DrawImageMask>RenderQuad;
 * sx,sy,sw,sh,���ɴ���ʾ��ͼƬ��Χ������Ҫ��ʾһ��ͼƬ200*200��ͼƬ��
 * 20��20��50��50�ߵ���ͼ�飬�����������20,20,50,50�����Ҫ��ʾԭͼ,������0
 * ����.����0����ʾ0,0,tex->w,tex->h��xscale,yscale�ǷŴ���С����1��ԭʼ������
 * ����Ŵ�һ��������2����С1����0.5��mask����ɫ���֣����ڶ�ͼƬ���л�ɫ��
 * �����͸��Ч���ȣ�Ĭ������ʾԭɫ��������texture->mask����
 * ����1:��200*200��ͼƬtex��ʾ����Ļ100��0�������Ŵ�һ����ʾ
 * RenderQuad(tex,0,0,0,0,100,0,2,2,0,tex->mask); 
 * ����2:��ͼƬtex��32��32��ʼ��64��,64�ߵĲ�����ʾ��40��80������ת90��
 * RenderQuad(tex,32,32,64,64,40,80,1,1,90,tex->mask); 
 * ����3:��ͼƬtex(tex��ʾģʽ��8888)��80��80��ʼ��64��,64�ߵĲ�����ʾ��100��100������͸����ʾ
 * RenderQuad(tex,80,80,64,64,100,100,1,1,0,MAKE_RGBA_8888(255,255,255,128)); 
 *@param image_p texture,ͼƬָ��
 *@param float sx,ͼƬx����
 *@param float sy,ͼƬy����
 *@param float sw,ͼƬ��
 *@param float sh,ͼƬ��,
 *@param float dx,��Ļx����
 *@param float dy,��Ļy����
 *@param float xscale,ͼƬx����Ŵ���С����
 *@param float yscale,ͼƬy����Ŵ���С����
 *@param float angle,��ת�Ƕ�
 *@param int mask,��ɫ����
 *@return ��
 */

void RenderHGEQuad(const hgeQuad * quad);

/**
 *����Ļ���ݱ�����image_p��
 *@return image_p,����image_pָ��,������NULL
 */
image_p ScreenToImage();
/**
 * ����һ��snapshotͼƬ���ļ�,���ͼƬ��png��ʽ��
 *@param const char* filename,����ͼƬ�ļ���
 *@return ��
 */
void ScreenShot(const char* filename);
/**
 *������Ļ��ɫ��r,g,b,a
 *@param uint8 r,����r����(0-255)
 *@param uint8 g,����g����(0-255)
 *@param uint8 b,����b����(0-255)
 *@param uint8 a,����a����(0-255)
 *@return uint32,��һ����Ļ��ɫֵ,���ú�GET_RGBA_8888�����������ɫ����
 */
uint32 SetScreenColor(uint8 r,uint8 g,uint8 b,uint8 a);
/**
 * ���õ�ǰ�Ļ�Ϸ�ʽ
 *@param int src_blend,
 *@param int des_blend,
 *@return
 */
void SetTexBlend(int src_blend, int des_blend);
/**
 *����Ϸ�ʽ�ָ�ΪĬ�Ϸ�ʽ
 *@return
 */
void ResetTexBlend();

#ifdef __cplusplus
}
#endif


#endif
