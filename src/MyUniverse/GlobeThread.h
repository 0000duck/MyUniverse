#pragma once

#include "GlbCore.h"
#include <vector>

using std::vector;

enum STORY_TYPE
{
    DDS,
    JPG,
    PNG,
    AVI,
    WMV,
    FOLDER,
    NONE
};

class CStoryPage
{
public:
    //ҳ���ļ��ṹ
    bool bEmpty;//ҳ���ļ�Ϊ��
    bool bMovie;//ҳ����һ������
    CString pagePath;//ҳ���ļ�������·����Ŀ¼+�ļ�����
    STORY_TYPE storyType;//DDS,JPG,PNG,AVI,WMV,FOLDER

    //����֡ר��
    vector<CString> FrameNames;//����֡��FOLDER����ʽʱ���������ÿһ֡���ļ���
    int nFrames;//����֡����֡��
    int nCurFrame;//��ǰ��֡��

    //��������
    bool bPlaying;//���Ŷ���
    int frameRate;//���������ٶ�

    //��ת����
    bool bRotating;//��ת
    bool bClockwise; //��ת����
    int rotationRate;//��ת�ٶ�
};

DWORD WINAPI GlobeThread(LPVOID lpParam);
DWORD WINAPI TimingThread(LPVOID lpParam);

void DrawStoryPage(GlbWindow mainWindow);
