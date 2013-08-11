#pragma once
//#include "windows.h"
#include <vector>
#include "GlbRot.h"
#include "opencv.hpp"
#include <afxwin.h>

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
    GlbRect(){}
    GlbRect(long left, long top, long width, long height)
    {
        m_left = left;
        m_top = top;
        m_width = width;
        m_height = height;
    }
};

class GlbWindow
{
public:
    HWND   m_hWnd;
    HDC    m_hDC;
    HGLRC  m_hRC;
};

GLBCORE_API int glbDetectScreen(vector<GlbRect> &screens);//�����Ļ�ĸ������ֱ��ʣ�������Ļ����

GLBCORE_API GlbImage glbLoadImage(const char* filename);    //����ͼ��֧��dds,jpg,bmp,png��
GLBCORE_API GlbImage glbLoadImageFromOpencv(IplImage* pImage, bool bMipmap = true);       //��OpenCVͼ��ת��
GLBCORE_API void glbReleaseImage(GlbImage* pImage);         //�ͷ�ͼ��

GLBCORE_API int glbCreateWindow(GlbWindow &window, GlbRect windowSize, bool fullscreen, bool mirror, HINSTANCE hInstance=0);
GLBCORE_API int glbCreateWindowMFC(GlbWindow &window, CRect rect, CWnd* parentWindow, bool mirror);
GLBCORE_API void glbSwitchWindow(GlbWindow window);
GLBCORE_API void glbDestoryWindow(GlbWindow window, HINSTANCE hInstance=0);

GLBCORE_API void glbClearWindow();                                    //��մ�������
GLBCORE_API void glbDrawImage(GlbImage image);              //�ڴ����л�ͼ,����û����ʾ

GLBCORE_API int glbUpdateWindow(GlbWindow window, int ms);                    //���´���������,����0��ʾ�����˳�������1��ʾ��������

GLBCORE_API void glbDrawLineOnGlobe(GlbPointGeo geoStartPoint, GlbRotmat GlobeRotMat, GlbPointGeo geoEndPoint, int layer);
GLBCORE_API void glbDrawLineOnScreen(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
GLBCORE_API void glbDrawGlobe(GlbImage Image, GlbRotmat GlobeRotMat);
GLBCORE_API void glbDrawTexture(
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

GLBCORE_API void glbDrawBelt(
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

//GLBCORE_API void glbShowFrameRate();

