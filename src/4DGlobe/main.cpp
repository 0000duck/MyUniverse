#include "GlbFrame.h"
#include <cstdio>

using namespace glb;

void main()
{
	vector<GlbRect> screens;	//�������Ļ��Ϣ
	glbDetectScreen(screens);	//�����Ļ�����ͷֱ���

	GlbRect rect(0,0,500,500);

    GlbWindow mainWindow;		//����
    glbCreateWindow(mainWindow, rect, ".\\calibmain.ini", true, false);	//����һ������
	
	GlbRotmat GlobeRotMat;		//������ת��
	glbCreateGlbRotmat(GlobeRotMat);	//��ʼ����ת�����������ϣ�

    glbSwitchWindow(mainWindow);	//�л������ڣ��ڶ�ȡͼ��֮ǰ��

	CurveRect curveRect(GLB_TEX_RECT, GlbPointGeo(0,0), true, GlbPointGeo(90,0), true, true, 360, 180);

	Object *earth = new Object(curveRect, "image\\earth.jpg");

	while(1)
	{
		glbClearWindow();	//�����������
		earth->Draw(GlobeRotMat, mainWindow.m_calib, 0);
		glbUpdateWindow(mainWindow,0);
	}
}
