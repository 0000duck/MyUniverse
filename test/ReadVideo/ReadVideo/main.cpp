#include "opencv.hpp"

void main()
{
    IplImage* Image=0;
    CvCapture* Cap = cvCaptureFromFile("�ȴ�����.WMV");
//    CvCapture* Cap = cvCaptureFromFile("input.avi");

    if(cvGrabFrame(Cap))
    do
    {
        Image = cvRetrieveFrame(Cap);
        cvNamedWindow("image");
        cvShowImage("image", Image);
        cvWaitKey();
    }
    while(cvGrabFrame(Cap));

}
