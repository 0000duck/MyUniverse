#include "stdafx.h"
#include "GlobeThread.h"
#include "MyUniverseDlg.h"
#include "StorytellerComp.h"

CRITICAL_SECTION g_StoryPage_CS;
CStoryPage g_StoryPage;

CRITICAL_SECTION g_GlobeRotMat_CS;
GlbRotmat g_GlobeRotMat;
GlbEularAngle g_StorytellerEularAngle;//����storyteller������ϵ
GlbEularAngle g_GlobeEularAngle;//����MyUniverse������ϵ
GlbEularAngle g_OffsetEularAngle;//������������ϵ�Ĳ��

extern int g_bMainThreadActive;//���̻߳ָʾ
extern int g_bGlbThreadActive;//�����ͼ�̻߳ָʾ
extern int g_bTimingThreadActive;//�����ͼ�̻߳ָʾ

DWORD WINAPI GlobeThread(LPVOID lpParam)
{
    g_bGlbThreadActive = 1;

    vector<GlbRect> screens;
    int nScreen = glbDetectScreen(screens);

    bool mirror =!!GetPrivateProfileInt("MyUniverseCfg", "MirrorDisplay", 0, ".\\config.ini");
    
    GlbWindow mainWindow;
    GlbWindow previewWindow;

    //����������
    if(nScreen >= 2)//��ʾ����������1��
    {
        glbCreateWindow(mainWindow, screens[1], true, mirror);//��ȫ����ʽ��ʾ����2����
    }
    else
    {
        GlbRect windowSize(0,0,500,500);
        glbCreateWindow(mainWindow, windowSize, false, mirror);//�Թ̶���С��ʾ�ڵ�һ����
    }

    //����Ԥ������
    CMyUniverseDlg* pWnd = (CMyUniverseDlg*)lpParam;
    CRect rect;
    pWnd->GetDlgItem(IDC_PREVIEW_WINDOW)->GetWindowRect(rect);
    pWnd->ScreenToClient(rect);
    glbCreateWindowMFC(previewWindow, rect, pWnd, 0);

    do
    {
        glbSwitchWindow(mainWindow);
        glbClearWindow();
        DrawStoryPage(mainWindow);
        glbUpdateWindow(mainWindow, 10);

        glbSwitchWindow(previewWindow);
        glbClearWindow();
        GlbImage Image = glbLoadImage("a001.dds");
        glbDrawImage(Image);
        glbReleaseImage(&Image);
        glbUpdateWindow(previewWindow, 10);

    }while(g_bMainThreadActive);

    glbDestoryWindow(mainWindow, 0);
    g_bGlbThreadActive = 0;
    return 1;
}

void DrawStoryPage(GlbWindow mainWindow)
{
    EnterCriticalSection(&g_StoryPage_CS);
    if(!g_StoryPage.bEmpty)
    {
        if(     g_StoryPage.storyType == DDS 
            ||  g_StoryPage.storyType == JPG
            ||  g_StoryPage.storyType == BMP
            ||  g_StoryPage.storyType == PNG )
        {
            static CString lastPath = "";
            static GlbImage Image = 0;

            if(strcmp(lastPath, g_StoryPage.pagePath))
            {
                if(Image)
                {
                    glbReleaseImage(&Image);
                }
                Image = glbLoadImage(g_StoryPage.pagePath);
            }
            EnterCriticalSection(&g_GlobeRotMat_CS);
            glbDrawGlobe(Image, g_GlobeRotMat);
            LeaveCriticalSection(&g_GlobeRotMat_CS);
            lastPath = g_StoryPage.pagePath;

        }
        else if(g_StoryPage.storyType == FOLDER) 
        {
            if(g_StoryPage.FrameNames.size()>0)
            {
                GlbImage Image = glbLoadImage(g_StoryPage.FrameNames[g_StoryPage.nCurFrame]);
                EnterCriticalSection(&g_GlobeRotMat_CS);
                glbDrawGlobe(Image, g_GlobeRotMat);
                LeaveCriticalSection(&g_GlobeRotMat_CS);
                glbReleaseImage(&Image);

                //ɾȥ�˾䣬����TimingThread�߳���
                //g_StoryPage.nCurFrame = (g_StoryPage.nCurFrame+g_StoryPage.bPlaying)%g_StoryPage.nFrames;
            }
        }
        else if(g_StoryPage.storyType == AVI
            || g_StoryPage.storyType == WMV) 
        {
            static  CvCapture* pCapture = NULL;
            static long snCurFrame = 0;
            static IplImage* pFrame = NULL;
            static CString videoPath = "";

            if(videoPath != g_StoryPage.pagePath)
            {
                if(pCapture)
	            {
		            cvReleaseCapture(&pCapture);
                }
                pCapture = cvCaptureFromFile(g_StoryPage.pagePath);
                if(!pCapture)
                {
                    AfxMessageBox("�޷���ȡ��Ƶ");
                    g_StoryPage.storyType = NONE;
                }
                videoPath = g_StoryPage.pagePath;
                g_StoryPage.nCurFrame = 0;
                snCurFrame = 0;
                pFrame = cvQueryFrame( pCapture );
            }
            
            if( snCurFrame < g_StoryPage.nCurFrame )
            {
                pFrame = cvQueryFrame( pCapture );
                if(pFrame == NULL)
                {
                    cvReleaseCapture(&pCapture);
                    pCapture = cvCaptureFromFile(g_StoryPage.pagePath);
                    pFrame = cvQueryFrame( pCapture );
                    g_StoryPage.nCurFrame = 0;
                }
                snCurFrame = g_StoryPage.nCurFrame;
            }
            GlbImage Image;
            if(pFrame)
            {
                Image = glbLoadImageFromOpencv(pFrame, false);
            }
            else
            {
                Image = 0;
            }

            EnterCriticalSection(&g_GlobeRotMat_CS);
            glbDrawGlobe(Image, g_GlobeRotMat);
            LeaveCriticalSection(&g_GlobeRotMat_CS);
            glbReleaseImage(&Image);
        }
    }
    LeaveCriticalSection(&g_StoryPage_CS);
}

DWORD WINAPI TimingThread(LPVOID lpParam)
{
    g_bTimingThreadActive = 1;
    LARGE_INTEGER tFreq={0};
	LARGE_INTEGER LastTimeCounter_playing;
	LARGE_INTEGER LastTimeCounter_rotating;
	LARGE_INTEGER TimeCounter;
	QueryPerformanceFrequency(&tFreq); 
	QueryPerformanceCounter(&LastTimeCounter_playing);
	QueryPerformanceCounter(&LastTimeCounter_rotating);

    while(g_bMainThreadActive)
    {
        QueryPerformanceCounter(&TimeCounter);

        //���¶���
        if(g_StoryPage.bPlaying && g_StoryPage.frameRate>0)
        {
            int frameRate = g_StoryPage.frameRate;
            if( frameRate<1)
            {
                frameRate = 1;
            }

	        if( TimeCounter.QuadPart - LastTimeCounter_playing.QuadPart > tFreq.QuadPart/frameRate)
	        {
		        LastTimeCounter_playing = TimeCounter;
                if(g_StoryPage.storyType == FOLDER)
                {
		            g_StoryPage.nCurFrame = (g_StoryPage.nCurFrame+g_StoryPage.bPlaying)%g_StoryPage.nFrames;
                }
                else// AVI or WMV
                {
                    g_StoryPage.nCurFrame ++;
                }
            }
        }
        else
        {
            LastTimeCounter_playing = TimeCounter;
        }

        //������ת
        if(g_StoryPage.bRotating)
        {
	        if( TimeCounter.QuadPart - LastTimeCounter_rotating.QuadPart > tFreq.QuadPart/30)
	        {
		        LastTimeCounter_rotating = TimeCounter;

                float rotationStep = g_StoryPage.rotationRate * 0.1f;
                if (g_StoryPage.bClockwise)
                {
                    rotationStep = -rotationStep;
                }

                EnterCriticalSection(&g_GlobeRotMat_CS);
                g_StorytellerEularAngle.m_3_Axis += rotationStep;
                if(g_StorytellerEularAngle.m_3_Axis>360)
                {
                    g_StorytellerEularAngle.m_3_Axis -= 360;
                }
                else if(g_StorytellerEularAngle.m_3_Axis<0)
                {
                    g_StorytellerEularAngle.m_3_Axis += 360;
                }
                //printf("Axis3 = %f\n", g_StorytellerEularAngle.m_3_Axis);
                StorytellerEular2GlobeEular(g_StorytellerEularAngle, g_OffsetEularAngle, g_GlobeEularAngle);
                glbEularAngle2Rotmat(g_GlobeEularAngle, g_GlobeRotMat);
                LeaveCriticalSection(&g_GlobeRotMat_CS);

                if(g_bMainThreadActive)
                {
                    //Sleep(1000);
                    CMyUniverseDlg* pDlg = (CMyUniverseDlg*)lpParam;                   
                    //�������淢���Զ�����Ϣ
                    //�ο���http://blog.csdn.net/zfpnuc/article/details/5944219
                    pDlg->SendMessage(WM_GLB_UPDATEDATA, FALSE);
                }
            }
        }
        else
        {
            LastTimeCounter_rotating = TimeCounter;
        }
        Sleep(1);
    }
    g_bTimingThreadActive = 0;
    return 1;
}
