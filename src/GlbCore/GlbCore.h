#pragma once

#ifdef GLBCORE_EXPORTS
#define GLBCORE_API __declspec(dllexport)
#else
#define GLBCORE_API __declspec(dllimport)
#endif

typedef unsigned int GlbImage;

GLBCORE_API GlbImage glbLoadImage(const char* filename);    //����ͼ��֧��dds,jpg,png��
GLBCORE_API void glbReleaseImage(GlbImage* pImage);         //�ͷ�ͼ��

GLBCORE_API int glbCreateWindow(HINSTANCE   hInstance);
GLBCORE_API void glbDestoryWindow(HINSTANCE   hInstance);

//void glbClearWindow();                                    //��մ�������
GLBCORE_API void glbDrawImage(GlbImage image);              //�ڴ����л�ͼ,����û����ʾ

GLBCORE_API int glbUpdateWindow(int ms);                    //���´���������