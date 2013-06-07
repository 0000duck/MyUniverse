#pragma once

#ifdef GLBROT_EXPORTS
#define GLBROT_API __declspec(dllexport)
#else
#define GLBROT_API __declspec(dllimport)
#endif

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

class GlbEularAngle
{
public:
    //��ת˳���ϸ��մ˽ṹ�е�˳��ŷ����->��ת��������z-x-z��˳��
    float m_1_Horz; //����ˮƽ������ת
    float m_2_Vert; //������0�Ⱦ��ߵ���ת
    float m_3_Axis; //����������ת
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

GLBROT_API float VectorNorm(GlbPoint3d Vec);
GLBROT_API void VectorNormalize(GlbPoint3d Vec, GlbPoint3d &Vec_dst);

GLBROT_API float DotMul(GlbPoint3d Vec1, GlbPoint3d Vec2);
GLBROT_API void CrossMul(GlbPoint3d Vec1, GlbPoint3d Vec2, GlbPoint3d &VecDST);
GLBROT_API void GlbRotmatMul(GlbRotmat mat1, GlbRotmat mat2, GlbRotmat &mat_dst);

//��γ���� ת��Ϊ ֱ������
GLBROT_API void glbPointGeo2PointRect(GlbPointGeo pGeo, GlbPoint3d &pRect);
//ֱ������ ת��Ϊ ��γ����
GLBROT_API void glbPointRect2PointGeo(GlbPoint3d pRect, GlbPointGeo &pGeo);

//ԭʼ�زģ����Σ����� ת��Ϊ �����ϵ�ֱ�����꣨�˺���û�е��ã�
GLBROT_API void Square2Sphere(GlbPoint2d pSquare, float TexWidth, float TexHeight, GlbPoint3d &pSphere);
//ֱ������ ת��Ϊ ԭʼ�ز�����
GLBROT_API void Sphere2Square(GlbPoint2d p1, float w, float h, GlbPoint3d &p0);

//�����ϵľ�γ���� ת��Ϊ ��Ļ�ϵľ�γ����
GLBROT_API void glbGlobePoint2ScreenPoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);
//��Ļ�ϵľ�γ���� ת��Ϊ �����ϵľ�γ����
GLBROT_API void glbScreenPoint2GlobePoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);

//ֱ������ ת��Ϊ Բ������
GLBROT_API void glbPointRect2PointRound(GlbPoint3d p2, float radius, GlbPoint2d &p3);
//Բ������ ת��Ϊ ֱ������
GLBROT_API void glbPointRound2PointRect(GlbPoint2d p3, float radius, GlbPoint3d &p2);

//���ɳ�ʼ��ת����
GLBROT_API void glbCreateGlbRotmat(GlbRotmat &r);
//������ת����
GLBROT_API void glbCloneGlbRotmat(GlbRotmat r, GlbRotmat &r_dst);

//������Ļ�ϵĵ�p1��p2����ת�Ƕ�
GLBROT_API float glbAngleBetweenPoints(GlbPoint3d p1, GlbPoint3d p2);
//������Ļ�ϵĵ�p1��p2����ת��
GLBROT_API void glbPivotBetweenPoints(GlbPoint3d p1, GlbPoint3d p2, GlbPoint3d &pivot);

//ŷ���� ת��Ϊ ��ת����
GLBROT_API void glbEularAngle2Rotmat(GlbEularAngle angle, GlbRotmat &r);
//��ת���� ת��Ϊ ��ת����
GLBROT_API void glbRotVector2RotMat(GlbPoint3d angle, GlbRotmat &r);
//��Ļ�ϵĵ�p1�ƶ���p2����ʾ����ת����
GLBROT_API void glbMovingPoints2RotMat(GlbPoint3d p1, GlbPoint3d p2, GlbRotmat &r);
//��ת���ת������ʾ����ת����
GLBROT_API void glbAnglePivot2RotMat(GlbPoint3d pivot, float angle, GlbRotmat &r);
//��p����pivot����תangle�Ƕ�
GLBROT_API void PivotRotPoint(GlbPoint3d p, GlbPoint3d pivot, float angle, GlbPoint3d &p_out);
//������ͼʱ�����������������ת��
GLBROT_API void CreateNormPivot(GlbPoint3d p, GlbPoint3d directPoint, bool bHeadDirect, GlbPoint3d &pivot_h, GlbPoint3d &pivot_v);//����p�㵽headPoint����ת�ᣬ�Լ��������������ת��

//float DistortR(float R);//���۾�ͷ����������
//float aDistortR(float R);//����������任
//
//int GetTopLayer(GlbPoint2d pTouch);
