#pragma once

typedef unsigned int GlbImage;

GlbImage glbLoadImage(const char* filename);  //����ͼ��֧��dds,jpg,png��
void glbReleaseImage(GlbImage* pImage);            //�ͷ�ͼ��

int glbCreateWindow(HINSTANCE   hInstance);
void glbDestoryWindow(HINSTANCE   hInstance);

//void glbClearWindow();                  //��մ�������
void glbDrawImage(GlbImage image);                    //�ڴ����л�ͼ,����û����ʾ
int glbUpdateWindow(int ms);                //���´���������