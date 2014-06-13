#include "glbImage.h"

namespace glb
{
	Image::Image(const char* filename)
	{
		imageID = glbLoadImage(filename, size);
	}

	void Image::Draw(const GlbRotmat &GlobeRotMat, const GlbCalib &calib, const CurveRect &curveRect, const int layer)
	{
		glbDrawTexture(
                    imageID,     //���ʵı��
                    GlobeRotMat,
                    calib,
                    curveRect.pGeo,       //��ͼ�����ĵ�
                    curveRect.bPointOnGlobe,     //���ĵ��������� globe����ϵ(true) or screen����ϵ(false)
                    curveRect.pGeoDirect, //��ͼ����Ĳο���
                    curveRect.bDirOnGlobe,       //����ο���ʵ�� globe����ϵ(true) or screen����ϵ(false)
                    curveRect.bHeadDirect,       //ͼƬ����ο���(ture) or ����ο���(false)
                    curveRect.width,            //��ͼ�Ŀ��(��λ:�Ƕ�)
                    curveRect.height,           //��ͼ�ĸ߶�(��λ:�Ƕ�)
                    layer,            //��ͼ���ڵĲ�
                    curveRect.mode);
	}
}