#ifndef ROTATION_H_H
#define ROTATION_H_H

#define PI (3.1415926f)
#define N_CALIB  (12) //�궨γ�ߵ�����


class GlbPointGeo   
{ 
public:
	float m_lat;//ά��
	float m_lng;//����
	GlbPointGeo()
	{
		m_lat = 0;
		m_lng = 0;
	}
	GlbPointGeo(float lat, float lng)
	{
		m_lat = lat;
		m_lng = lng;
	}
	GlbPointGeo &operator =(GlbPointGeo p)    //Ҫ�޸Ķ������ݳ�Ա������ֵ����Ϊ���������
	{
		m_lat = p.m_lat;
		m_lng = p.m_lng;
		return *this;            //�����޸ĺ����ֵ����
	}
}; 

class GlbPoint2d   
{ 
public:
	float m_x;
	float m_y;
	GlbPoint2d()
	{
		m_x = 0;
		m_y = 0;
	}
	GlbPoint2d(float x, float y)
	{
		m_x = x;
		m_y = y;
	}
	GlbPoint2d &operator =(GlbPoint2d p)    //Ҫ�޸Ķ������ݳ�Ա������ֵ����Ϊ���������
	{
		m_x = p.m_x;
		m_y = p.m_y;
		return *this;            //�����޸ĺ����ֵ����
	}
}; 

class GlbPoint3d 
{ 
public:
	float m_x;
	float m_y;
	float m_z;
	GlbPoint3d()
	{
		m_x = 0;
		m_y = 0;
		m_z = 0;
	}
	GlbPoint3d(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}
	GlbPoint3d &operator =(GlbPoint3d p)    //Ҫ�޸Ķ������ݳ�Ա������ֵ����Ϊ���������
	{
		m_x = p.m_x;
		m_y = p.m_y;
		m_z = p.m_z;
		return *this;            //�����޸ĺ����ֵ����
	}
	GlbPoint3d &operator +=(GlbPoint3d p)    //Ҫ�޸Ķ������ݳ�Ա������ֵ����Ϊ���������
	{
		m_x += p.m_x;
		m_y += p.m_y;
		m_z += p.m_z;
		return *this;            //�����޸ĺ����ֵ����
	}	
	
	GlbPoint3d &operator *=(float f)    //Ҫ�޸Ķ������ݳ�Ա������ֵ����Ϊ���������
	{
		m_x *= f;
		m_y *= f;
		m_z *= f;
		return *this;            //�����޸ĺ����ֵ����
	}
}; 

class GlbRotmat 
{ 
public:
	float r11;
	float r12;
	float r13;
	float r21;
	float r22;
	float r23;
	float r31;
	float r32;
	float r33;
}; 

class CMove
{
public:
	GlbPoint2d m_pFrom;
	GlbPoint2d m_pTo;
	long m_nTrackID;
};

float VectorNorm(GlbPoint3d Vec);
void VectorNormalize(GlbPoint3d Vec, GlbPoint3d &Vec_dst);

float DotMul(GlbPoint3d Vec1, GlbPoint3d Vec2);
void CrossMul(GlbPoint3d Vec1, GlbPoint3d Vec2, GlbPoint3d &VecDST);
void GlbRotmatMul(GlbRotmat mat1, GlbRotmat mat2, GlbRotmat &mat_dst);

void Geo2Rect(GlbPointGeo pGeo, GlbPoint3d &pRect);
void Rect2Geo(GlbPoint3d pRect, GlbPointGeo &pGeo);

void Square2Sphere(GlbPoint2d pSquare, float TexWidth, float TexHeight, GlbPoint3d &pSphere);
void Sphere2Square(GlbPoint2d p1, float w, float h, GlbPoint3d &p0);

void Sphere2Sphere(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);
void aSphere2Sphere(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);

void Sphere2Round(GlbPoint3d p2, float radius, GlbPoint2d &p3);
void Round2Sphere(GlbPoint2d p3, float radius, GlbPoint3d &p2);

void CreateGlbRotmat(GlbRotmat &r);//���ɳ�ʼ��ת����
void CloneGlbRotmat(GlbRotmat r, GlbRotmat &r_dst);

float Point2Angle(GlbPoint3d p1, GlbPoint3d p2);//����p1��p2����ת�Ƕ�
void Point2Pivot(GlbPoint3d p1, GlbPoint3d p2, GlbPoint3d &pivot, bool bNormalize);//����p1��p2����ת��

void AngleGlbRotmat(GlbPoint3d angle, GlbRotmat &r);//angle����������Ϊ��x����y����z��ת�ĽǶȣ����ν�����ת�ľ���
void PointGlbRotmat(GlbPoint3d p1, GlbPoint3d p2, GlbRotmat &r);//�����p1��p2����ת����
void PivotRotmat(GlbPoint3d pivot, float angle, GlbRotmat &rot);
void PivotRotPoint(GlbPoint3d p, GlbPoint3d pivot, float angle, GlbPoint3d &p_out);//��p����pivot����תangle�Ƕ�
void CreateNormPivot(GlbPoint3d p, GlbPoint3d directPoint, bool bHeadDirect, GlbPoint3d &pivot_h, GlbPoint3d &pivot_v);//����p�㵽headPoint����ת�ᣬ�Լ��������������ת��

//float DistortR(float R);//���۾�ͷ����������
//float aDistortR(float R);//����������任
//
//int GetTopLayer(GlbPoint2d pTouch);

#endif