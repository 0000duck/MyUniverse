
// My_MFC_OpenGL_test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy_MFC_OpenGL_testApp:
// �йش����ʵ�֣������ My_MFC_OpenGL_test.cpp
//

class CMy_MFC_OpenGL_testApp : public CWinApp
{
public:
	CMy_MFC_OpenGL_testApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy_MFC_OpenGL_testApp theApp;