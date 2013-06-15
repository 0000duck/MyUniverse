#pragma once
#include "windows.h"
#include <vector>
#include "GlbRot.h"

#ifdef GLBCORE_EXPORTS
#define GLBCORE_API __declspec(dllexport)
#else
#define GLBCORE_API __declspec(dllimport)
#endif

using namespace std;

typedef unsigned int GlbImage;

enum GlbTexMode
{
    GLB_TEX_RECT,
    GLB_TEX_BELT
};

class GlbRect
{
public:
    long m_left;
    long m_top;
    long m_width;
    long m_height;
};

GLBCORE_API int glbDetectScreen(vector<GlbRect> &screens);

GLBCORE_API GlbImage glbLoadImage(const char* filename);    //����ͼ��֧��dds,jpg,png��
GLBCORE_API void glbReleaseImage(GlbImage* pImage);         //�ͷ�ͼ��

GLBCORE_API int glbCreateWindow(HINSTANCE   hInstance);
GLBCORE_API void glbDestoryWindow(HINSTANCE   hInstance);

GLBCORE_API void glbClearWindow();                                    //��մ�������
GLBCORE_API void glbDrawImage(GlbImage image);              //�ڴ����л�ͼ,����û����ʾ

GLBCORE_API int glbUpdateWindow(int ms);                    //���´���������,����0��ʾ�����˳�������1��ʾ��������

GLBCORE_API void DrawLineOnGlobe(GlbPointGeo geoStartPoint, GlbRotmat GlobeRotMat, GlbPointGeo geoEndPoint, int layer);
GLBCORE_API void DrawLineOnScreen(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
GLBCORE_API void DrawGlobe(GlbImage Image, GlbRotmat GlobeRotMat);
GLBCORE_API void DrawTexture(
                    GlbImage Image,     //���ʵı��
                    GlbRotmat GlobeRotMat,
                    GlbPointGeo pGeo,       //��ͼ�����ĵ�
                    bool bPointOnGlobe,     //���ĵ��������� globe����ϵ(true) or screen����ϵ(false)
                    GlbPointGeo pGeoDirect, //��ͼ����Ĳο���
                    bool bDirOnGlobe,       //����ο���ʵ�� globe����ϵ(true) or screen����ϵ(false)
                    bool bHeadDirect,       //ͼƬ����ο���(ture) or ����ο���(false)
                    float width,            //��ͼ�Ŀ��(��λ:�Ƕ�)
                    float height,           //��ͼ�ĸ߶�(��λ:�Ƕ�)
                    float layer,            //��ͼ���ڵĲ�
                    GlbTexMode mode,
                    GlbPoint3d &pClose      //������ͼ���Ͻǵ�����
);

GLBCORE_API void DrawBelt(
                GlbImage Image,
                GlbRotmat GlobeRotMat,
                GlbPointGeo pGeo,
                bool bPointOnGlobe,
                GlbPointGeo pGeoDirect,	
                bool bDirOnGlobe,		
                bool bHeadDirect,		
                float width,			
                float height,			
                float layer
    );

GLBCORE_API void ShowFrameRate();

