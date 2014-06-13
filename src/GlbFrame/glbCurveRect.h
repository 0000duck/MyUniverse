#pragma once
#include "GlbCore.h"
#include "GlbRot.h"

namespace glb
{	class CurveRect
	{
	public:
		GlbTexMode mode;
		GlbPointGeo pGeo;       //��ͼ�����ĵ�
		bool bPointOnGlobe;     //���ĵ��������� globe����ϵ(true) or screen����ϵ(false)
		GlbPointGeo pGeoDirect; //��ͼ����Ĳο���
		bool bDirOnGlobe;       //����ο���ʵ�� globe����ϵ(true) or screen����ϵ(false)
		bool bHeadDirect;       //ͼƬ����ο���(ture) or ����ο���(false)
		float width;            //��ͼ�Ŀ��(��λ:�Ƕ�)
		float height;           //��ͼ�ĸ߶�(��λ:�Ƕ�)

	public:
		CurveRect(
			GlbTexMode mode, 
			GlbPointGeo pGeo,
			bool bPointOnGlobe,
			GlbPointGeo pGeoDirect,
			bool bDirOnGlobe,
			bool bHeadDirect,
			float width,
			float height);
	};
}