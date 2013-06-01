#pragma once
#include "gl\glut.h"
#include "gl\glaux.h"
#include "gl\gl.h"

GLvoid KillGLWindow(GLvoid);
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
int InitGL(GLvoid);

LRESULT CALLBACK WndProc(	HWND	hWnd,					// ���ڵľ��
				UINT	uMsg,					// ���ڵ���Ϣ
				WPARAM	wParam,					// ���ӵ���Ϣ����
				LPARAM	lParam);					// ���ӵ���Ϣ����

void DrawCalib();

DWORD WINAPI DrawGLScene(LPVOID lpParam);