#include "stdafx.h"
#include "GLShow.h"
#include <windows.h>		// Windows��ͷ�ļ�
#include <math.h>
#include "image.h"
#include "draw.h"
#include "rotation.h"

extern GlbRotmat g_GlobeRotMat;

HGLRC           hRC=NULL;							// ������ɫ��������
HDC             hDC=NULL;							// OpenGL��Ⱦ��������
HWND            hWnd=NULL;							// �������ǵĴ��ھ��
HINSTANCE       hInstance;							// ��������ʵ��

bool	keys[256];								// ������̰���������
bool	active=TRUE;								// ���ڵĻ��־��ȱʡΪTRUE
bool	fullscreen=TRUE;	

int g_rot_x = 0;
int g_rot_y = 0;
int g_rot_z = 0;


GLvoid KillGLWindow(GLvoid)							// �������ٴ���
{

	if (fullscreen)								// ���Ǵ���ȫ��ģʽ��?
	{

		ChangeDisplaySettings(NULL,0);					// �ǵĻ����л�������
		ShowCursor(TRUE);						// ��ʾ���ָ��
	}

	if (hRC)								// ����ӵ��OpenGL��Ⱦ��������?
	{

		if (!wglMakeCurrent(NULL,NULL))					// �����ܷ��ͷ�DC��RC������?
		{

			MessageBox(NULL,"�ͷ�DC��RCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))					// �����ܷ�ɾ��RC?
		{

			MessageBox(NULL,"�ͷ�RCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;							// ��RC��Ϊ NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// �����ܷ��ͷ� DC?
	{
		MessageBox(NULL,"�ͷ�DCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;							// �� DC ��Ϊ NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// �ܷ����ٴ���?
	{
		MessageBox(NULL,"�ͷŴ��ھ��ʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;							// �� hWnd ��Ϊ NULL
	}

	if (!UnregisterClass("OpenG",hInstance))				// �ܷ�ע����?
	{
		MessageBox(NULL,"����ע�������ࡣ","�رմ���",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							// �� hInstance ��Ϊ NULL
	}
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{

	GLuint		PixelFormat;						// �������ƥ��Ľ��

	WNDCLASS	wc;							// ������ṹ

	DWORD		dwExStyle;						// ��չ���ڷ��
	DWORD		dwStyle;						// ���ڷ��

	RECT WindowRect;							// ȡ�þ��ε����ϽǺ����½ǵ�����ֵ
	WindowRect.left=(long)0;						// ��Left   ��Ϊ 0
	WindowRect.right=(long)width;						// ��Right  ��ΪҪ��Ŀ��
	WindowRect.top=(long)0;							// ��Top    ��Ϊ 0
	WindowRect.bottom=(long)height;						// ��Bottom ��ΪҪ��ĸ߶�

	fullscreen=fullscreenflag;						// ����ȫ��ȫ����־

	hInstance		= GetModuleHandle(NULL);			// ȡ�����Ǵ��ڵ�ʵ��
	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// �ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc������Ϣ
	wc.cbClsExtra		= 0;						// �޶��ⴰ������
	wc.cbWndExtra		= 0;						// �޶��ⴰ������
	wc.hInstance		= hInstance;					// ����ʵ��
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);			// װ��ȱʡͼ��
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			// װ�����ָ��
	wc.hbrBackground	= NULL;						// GL����Ҫ����
	wc.lpszMenuName		= NULL;						// ����Ҫ�˵�
	wc.lpszClassName	= "OpenG";					// �趨������

	if (!RegisterClass(&wc))						// ����ע�ᴰ����
	{
		MessageBox(NULL,"ע�ᴰ��ʧ��","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// �˳�������FALSE
	}

	if (fullscreen)								// Ҫ����ȫ��ģʽ��?
	{

		DEVMODE dmScreenSettings;						// �豸ģʽ
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));			// ȷ���ڴ����Ϊ��
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			// Devmode �ṹ�Ĵ�С
		dmScreenSettings.dmPelsWidth	= width;				// ��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight	= height;				// ��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel	= bits;					// ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// ����������ʾģʽ�����ؽ����ע: CDS_FULLSCREEN ��ȥ��״̬����
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{

			// ��ģʽʧ�ܣ��ṩ����ѡ��˳����ڴ��������С�
			if (MessageBox(NULL,"ȫ��ģʽ�ڵ�ǰ�Կ�������ʧ�ܣ�\nʹ�ô���ģʽ��","NeHe G",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{

				fullscreen=FALSE;				// ѡ�񴰿�ģʽ(Fullscreen=FALSE)
			}
			else
			{

				// ����һ���Ի��򣬸����û��������
				MessageBox(NULL,"���򽫱��ر�","����",MB_OK|MB_ICONSTOP);
				return FALSE;					//  �˳������� FALSE
			}
		}
	}

	if (fullscreen)								// �Դ���ȫ��ģʽ��?
	{

		dwExStyle=WS_EX_APPWINDOW;					// ��չ������
		dwStyle=WS_POPUP;						// ������
		//ShowCursor(FALSE);						// �������ָ��
	}
	else
	{

		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// ��չ������
		dwStyle=WS_OVERLAPPEDWINDOW;					//  ������
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// �������ڴﵽ����Ҫ��Ĵ�С

	if (!(hWnd=CreateWindowEx(	dwExStyle,				// ��չ������
					"OpenG",				// ������
					title,					// ���ڱ���
					WS_CLIPSIBLINGS |			// ����Ĵ���������
					WS_CLIPCHILDREN |			// ����Ĵ���������
					dwStyle,				// ѡ��Ĵ�������
					0, 0,					// ����λ��
					WindowRect.right-WindowRect.left,	// ��������õĴ��ڿ��
					WindowRect.bottom-WindowRect.top,	// ��������õĴ��ڸ߶�
					NULL,					// �޸�����
					NULL,					// �޲˵�
					hInstance,				// ʵ��
					NULL)))					// ����WM_CREATE�����κζ���

	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"���ܴ���һ�������豸������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=					// /pfd ���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// ������ʽ�������Ĵ�С
		1,								// �汾��
		PFD_DRAW_TO_WINDOW |						// ��ʽ֧�ִ���
		PFD_SUPPORT_OPENGL |						// ��ʽ����֧��OpenGL
		PFD_DOUBLEBUFFER,						// ����֧��˫����
		PFD_TYPE_RGBA,							// ���� RGBA ��ʽ
		bits,								// ѡ��ɫ�����
		0, 0, 0, 0, 0, 0,						// ���Ե�ɫ��λ
		0,								// ��Alpha����
		0,								// ����Shift Bit
		0,								// ���ۼӻ���
		0, 0, 0, 0,							// ���Ծۼ�λ
		16,								// 16λ Z-���� (��Ȼ���)
		0,								// ���ɰ建��
		0,								// �޸�������
		PFD_MAIN_PLANE,							// ����ͼ��
		0,								// Reserved
		0, 0, 0								// ���Բ�����
	};

	if (!(hDC=GetDC(hWnd)))							// ȡ���豸��������ô?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"���ܴ���һ����ƥ������ظ�ʽ","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				// Windows �ҵ���Ӧ�����ظ�ʽ����?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"�����������ظ�ʽ","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// �ܹ��������ظ�ʽô?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"�����������ظ�ʽ","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))					// �ܷ�ȡ����ɫ������?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"���ܴ���OpenGL��Ⱦ������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))						// ���Լ�����ɫ������
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"���ܼ��ǰ��OpenGL��Ȼ������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// ��ʾ����
	SetForegroundWindow(hWnd);						// ����������ȼ�
	SetFocus(hWnd);								// ���ü��̵Ľ������˴���
	ReSizeGLScene(width, height);						// ����͸�� GL ��Ļ

	if (!InitGL())								// ��ʼ���½���GL����
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	return TRUE;								// �ɹ�
}

int InitGL(GLvoid)								// �˴���ʼ��OpenGL������������
{
	glEnable(GL_TEXTURE_2D);						// ��������ӳ��
	glShadeModel(GL_SMOOTH);						// ������Ӱƽ��
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					// ��ɫ����
	glClearDepth(1.0f);							// ������Ȼ���
	glEnable(GL_DEPTH_TEST);						// ������Ȳ���
	glDepthFunc(GL_LEQUAL);							// ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// ������ϸ��͸������

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);					// Set The Blending Function For Translucency
	
	unsigned int iTexture;
	LoadTexture("earth.jpg", iTexture);
	CreateGlbRotmat(g_GlobeRotMat);



	MSG	msg;
	bool done = false;
	while(!done)								// ����ѭ��ֱ�� done=TRUE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// ����Ϣ�ڵȴ���?
		{
			if (msg.message==WM_QUIT)				// �յ��˳���Ϣ?
			{
				done=TRUE;					// �ǣ���done=TRUE
			}
			else							// ���ǣ���������Ϣ
			{
				TranslateMessage(&msg);				// ������Ϣ
				DispatchMessage(&msg);				// ������Ϣ
			}
		}
		else								// ���û����Ϣ
		{
			// ���Ƴ���������ESC��������DrawGLScene()���˳���Ϣ
			if (active)						// ���򼤻��ô?
			{
				glLoadIdentity();							// ���õ�ǰ��ģ�͹۲����
				glMatrixMode( GL_PROJECTION );
				glLoadIdentity();
				float W_H_Rate = 200/(float)200;
				glOrtho( -W_H_Rate, W_H_Rate, -1, 1, -10, 20);
				glMatrixMode( GL_MODELVIEW );
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				GlbPoint3d angle(g_rot_x/100.0*PI*2,g_rot_y/100.0*PI*2,g_rot_z/100.0*PI*2);
				AngleGlbRotmat(angle, g_GlobeRotMat);

				DrawGlobe(iTexture);
				//DrawCalib();
				SwapBuffers(hDC);			// �������� (˫����)
				Sleep(10);
			}
		}
	}

	return TRUE;								// ��ʼ�� OK
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)				// ����OpenGL���ڴ�С
{
	if (height==0)								// ��ֹ�����
	{
		height=1;							// ��Height��Ϊ1
	}

	glViewport(0, 0, width, height);					// ���õ�ǰ���ӿ�

	glMatrixMode(GL_PROJECTION);						// ѡ��ͶӰ����
	glLoadIdentity();							// ����ͶӰ����

	// �����ӿڵĴ�С
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);						// ѡ��ģ�͹۲����
	glLoadIdentity();							// ����ģ�͹۲����
}

LRESULT CALLBACK WndProc(	HWND	hWnd,					// ���ڵľ��
				UINT	uMsg,					// ���ڵ���Ϣ
				WPARAM	wParam,					// ���ӵ���Ϣ����
				LPARAM	lParam)					// ���ӵ���Ϣ����
{

	switch (uMsg)								// ���Windows��Ϣ
	{
		case WM_CREATE:
		{
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
		}
		case WM_ACTIVATE:						// ���Ӵ��ڼ�����Ϣ
		{
			if (!HIWORD(wParam))					// �����С��״̬
			{
				active=TRUE;					// �����ڼ���״̬
			}
			else
			{
				active=FALSE;					// �����ټ���
			}

			return 0;						// ������Ϣѭ��
		}

		case WM_SYSCOMMAND:						// ϵͳ�ж�����
		{
			switch (wParam)						// ���ϵͳ����
			{
				case SC_SCREENSAVE:				// ����Ҫ����?
				case SC_MONITORPOWER:				// ��ʾ��Ҫ����ڵ�ģʽ?
				return 0;					// ��ֹ����
			}
			break;							// �˳�
		}

		case WM_CLOSE:							// �յ�Close��Ϣ?
		{
			PostQuitMessage(0);					// �����˳���Ϣ
			return 0;						// ����
		}

		case WM_KEYDOWN:						// �м�����ô?
		{
			return 0;						// ����
		}

		case WM_KEYUP:							// �м��ſ�ô?
		{
			return 0;						// ����
		}

		case WM_SIZE:							// ����OpenGL���ڴ�С
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));		// LoWord=Width,HiWord=Height
			return 0;						// ����
		}
	}

	// �� DefWindowProc��������δ�������Ϣ��
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void DrawCalib()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// �����Ļ����Ȼ���
	glLoadIdentity();											// ���õ�ǰ��ģ�͹۲����
	glBindTexture(GL_TEXTURE_2D, 0);

	glPointSize(2.0f);
	GLfloat x,y,angle;

	int n= g_rot_x+2;

	for(int nLatitude=0; nLatitude<=n; nLatitude++)
	{
		glColor3f(1.0f,1.0f,1.0f);

		for(angle=0.0f;angle<2*PI;angle+=0.01f)
		{
			glBegin(GL_POINTS);
			float R = 1.0/n*nLatitude;
			x=R * sin(angle);
			y=R * cos(angle);
			glVertex2f(x,y);
			glEnd();			
		}
	}
}

DWORD WINAPI DrawGLScene(LPVOID lpParam)
{
	CreateGLWindow("MyUniverse",200,200,16,false);//glOrtho�Ĳ�����˶�Ӧ
	return 1;
}