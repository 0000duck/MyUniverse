#include "GlbTouch.h"
#include "TuioDump.h"

void glbListenTouchSignal(GlbWindow &window, int port)
{
	window.m_TuioPort = port;

	HANDLE hHandle = CreateThread(0, 0, TuioThread, (LPVOID)(&window),0,0);//����TUIO�����߳�

	//SuspendThread(hHandle);        //��ͣ�������߳�
	//SetThreadPriority(hHandle, THREAD_PRIORITY_HIGHEST); //�趨�̵߳����ȼ���
	//ResumeThread(hHandle);           //���������߳�
}