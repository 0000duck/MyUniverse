
// MFC_OpenGL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_OpenGLApp:
// �йش����ʵ�֣������ MFC_OpenGL.cpp
//

class CMFC_OpenGLApp : public CWinApp
{
public:
	CMFC_OpenGLApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_OpenGLApp theApp;