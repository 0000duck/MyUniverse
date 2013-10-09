#include "opencv.hpp"
#include "CvxText.h"
#include <string.h>

using namespace std;

void main()
{
    //CvxText text("simhei.ttf");
    CvxText text("simsun.ttc");
    const wchar_t *msg = L"��OpenCV��  ������֣�";//����������ַ���ǰ��L

    float p = 1;
    CvScalar TypeSize = cvScalar( 
        40,     // �����С
        1,      // �ո�Ĵ�С����
        0.1,    // �����С����
        0       // ��ת�Ƕ�(��֧��)
        );
    bool bUnderline = true;

    text.setFont(NULL, &TypeSize, &bUnderline, &p);

    CvPoint Point = cvPoint(10,10);
    CvFont font = cvFont(10, 10);

    IplImage *pFrame = cvCreateImage(cvSize(500,500), 8, 3);
    cvZero(pFrame);
    text.putText(pFrame, msg, cvPoint(100, 150), CV_RGB(0,255,0));

    cvNamedWindow("show",1);
    cvShowImage("show",pFrame);
    cvWaitKey(0);

    cvDestroyWindow("show");
    cvReleaseImage(&pFrame);
}