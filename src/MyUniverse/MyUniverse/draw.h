#ifndef DRAW_H_H_H
#define DRAW_H_H_H

#include "rotation.h"
#include "gl/glut.h"
#include <string>

using namespace std;

#define FACET_SCACLE_IN_ANGLE (6) //ÿ����Ƭռ����С�Ƕ�
#define MAX_FACET_SHOW_THRESHOLD (0.4f)//����ʾ�������Ƭ��size

//#define GLB_TEX_RECT (1)
//#define GLB_TEX_BELT (2)

enum GlbTexMode
{
	GLB_TEX_RECT,
	GLB_TEX_BELT
};

class GlbObject
{
private:
	GLuint TextureID;					//���ʵı��
	string filename;					//���ʵ��ļ���
	GlbPointGeo m_CenterPoint;			//��ͼ�����ĵ�
	bool m_bCenterPointOnGlobe;			//���ĵ��������� globe����ϵ(true) or screen����ϵ(false)
	GlbPointGeo m_DirectPoint;			//��ͼ����Ĳο���
	bool m_bDirectPointOnGlobe;			//����ο���ʵ�� globe����ϵ(true) or screen����ϵ(false)
	bool m_bHeadDirect;					//ͼƬ����ο���(ture) or ����ο���(false)
	float m_width;						//��ͼ�Ŀ��(��λ:�Ƕ�)
	float m_height;						//��ͼ�ĸ߶�(��λ:�Ƕ�)
	unsigned int m_layer;				//��ͼ���ڵĲ�
	GlbTexMode m_mode;
};

// class MovieID
// {
// public:
// 	unsigned int ID;
// 	std::string filename;
// 
// public:
// 	//MovieID();
// };

void DrawLineOnGlobe(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
void DrawLineOnScreen(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
void DrawGlobe(GLuint texture);
void DrawTexture(
				 GLuint TextureName,	//���ʵı��
				 GlbPointGeo pGeo,			//��ͼ�����ĵ�
				 bool bPointOnGlobe,	//���ĵ��������� globe����ϵ(true) or screen����ϵ(false)
				 GlbPointGeo pGeoDirect,	//��ͼ����Ĳο���
				 bool bDirOnGlobe,		//����ο���ʵ�� globe����ϵ(true) or screen����ϵ(false)
				 bool bHeadDirect,		//ͼƬ����ο���(ture) or ����ο���(false)
				 float width,			//��ͼ�Ŀ��(��λ:�Ƕ�)
				 float height,			//��ͼ�ĸ߶�(��λ:�Ƕ�)
				 float layer,			//��ͼ���ڵĲ�
				 GlbTexMode mode,
				 GlbPoint3d &pClose		//������ͼ���Ͻǵ�����
				 );

void DrawBelt(GLuint TextureName,
			  GlbPointGeo pGeo,
			  bool bPointOnGlobe,
			  GlbPointGeo pGeoDirect,	
			  bool bDirOnGlobe,		
			  bool bHeadDirect,		
			  float width,			
			  float height,			
			  float layer);

void DrawMovie(
				GLuint videoID,
				const char* videoPath,
				GlbPointGeo pGeo,
				bool bPointOnGlobe,
				GlbPointGeo pGeoDirect,	
				bool bDirOnGlobe,		
				bool bHeadDirect,		
				float width,			
				float height,			
				float layer,
				GlbPoint3d &pClose);

void ShowFrameRate();

#endif