﻿#pragma once
//#include "windows.h"
#include <vector>
#include <list> 
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

class GlbMove
{
public:
	GlbPoint2d m_pFrom;
	GlbPoint2d m_pTo;
	long m_nTrackID;
};

class GlbWindow
{
public:
    HWND	m_hWnd;
    HDC		m_hDC;
    HGLRC	m_hRC;
	int		m_width;
	int		m_height;
    GlbCalib m_calib;
	int     m_TuioPort;

	CRITICAL_SECTION m_touch_cs; //触摸信号互斥临界区
	vector<GlbPoint2d> m_touchSignal;
	vector<GlbMove> m_moveSignal;
public:
	GlbWindow()
	{
		InitializeCriticalSection(&m_touch_cs);
	}
	~GlbWindow()
	{
		//DeleteCriticalSection(&m_touch_cs);
	}
	void EnterTouchCS()
	{
		EnterCriticalSection(&m_touch_cs);
	}
	void LeaveTouchCS()
	{
		LeaveCriticalSection(&m_touch_cs);
	}
};

GLBCORE_API int glbDetectScreen(vector<GlbRect> &screens);//检测屏幕的个数及分辨率，返回屏幕个数

GLBCORE_API GlbImage glbLoadImage(const char* filename);    //载入图像（支持dds,jpg,bmp,png）
GLBCORE_API GlbImage glbLoadImageFromOpencv(IplImage* pImage, bool bMipmap = true);       //从OpenCV图像转换
GLBCORE_API void glbReleaseImage(GlbImage* pImage);         //释放图像

GLBCORE_API int glbCreateWindow(GlbWindow &window, GlbRect windowSize, char *calibFileName, bool fullscreen, bool mirror, HINSTANCE hInstance=0);
GLBCORE_API int glbCreateWindowMFC(GlbWindow &window, CRect rect, char *calibFileName, CWnd* parentWindow, bool mirror);
GLBCORE_API void glbSwitchWindow(GlbWindow window);
GLBCORE_API void glbDestoryWindow(GlbWindow window, HINSTANCE hInstance=0);
GLBCORE_API void glbPopTouchSignal(GlbWindow &window, vector<GlbMove> &move, vector<GlbPoint2d> &touch); 

GLBCORE_API void glbClearWindow();                                    //清空窗口内容
GLBCORE_API void glbDrawImage(GlbImage image);              //在窗口中画图,但并没有显示

GLBCORE_API int glbUpdateWindow(GlbWindow window, int ms);                    //更新窗口中内容,返回0表示窗口退出，返回1表示正常运行

GLBCORE_API void glbDrawLine(	GlbPointGeo geoStartPoint, bool bStartPointOnGlobe, 
										GlbPointGeo geoEndPoint, bool bEndPointOnGlobe, 
										GlbRotmat GlobeRotMat, GlbCalib calib, int layer);
GLBCORE_API void glbDrawLineOnScreen(GlbCalib calib, GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer);
GLBCORE_API void glbDrawCircle(GlbPointGeo geoCenterPoint, bool bCenterPointOnGlobe, float radius, GlbRotmat GlobeRotMat, GlbCalib calib,  int layer);

GLBCORE_API void glbDrawGlobe(GlbImage Image, GlbRotmat GlobeRotMat, GlbCalib calib);
GLBCORE_API void glbDrawTexture(
                    GlbImage Image,     //材质的编号
                    GlbRotmat GlobeRotMat,
                    GlbCalib calib,
                    GlbPointGeo pGeo,       //贴图的中心点
                    bool bPointOnGlobe,     //中心点坐标是在 globe坐标系(true) or screen坐标系(false)
                    GlbPointGeo pGeoDirect, //贴图方向的参考点
                    bool bDirOnGlobe,       //方向参考点实在 globe坐标系(true) or screen坐标系(false)
                    bool bHeadDirect,       //图片朝向参考点(ture) or 背向参考点(false)
                    float width,            //贴图的宽度(单位:角度)
                    float height,           //贴图的高度(单位:角度)
                    float layer,            //贴图所在的层
                    GlbTexMode mode
);

GLBCORE_API void glbDrawBelt(
                GlbImage Image,
                GlbRotmat GlobeRotMat,
                GlbCalib calib,
                GlbPointGeo pGeo,
                bool bPointOnGlobe,
                GlbPointGeo pGeoDirect,	
                bool bDirOnGlobe,		
                bool bHeadDirect,		
                float width,			
                float height,			
                float layer
    );

GLBCORE_API int glbGetTopLayer(GlbWindow &window, GlbPoint2d pTouch);

GLBCORE_API void glbListenTouchSignal(GlbWindow &window, int port);

//GLBCORE_API void glbShowFrameRate();
