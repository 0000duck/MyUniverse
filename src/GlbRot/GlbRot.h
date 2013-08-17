#pragma once

#ifdef GLBROT_EXPORTS
#define GLBROT_API __declspec(dllexport)
#else
#define GLBROT_API __declspec(dllimport)
#endif

#define PI (3.1415926f)
//#define N_CALIB  (12) //�궨γ�ߵ�����


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

class GlbCalib
{
    #define N_CALIB  (12) //�궨γ�ߵ�����
public:
    float latitude[N_CALIB+1];
};


typedef GlbPoint3d GlbPivot;
typedef GlbPoint2d GlbPointRound;

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

//���������ĳ��ȣ�ģ��
GLBROT_API float glbVectorNorm(GlbPoint3d Vec);
//��������Ϊ��λ����
GLBROT_API void glbVectorNormalize(GlbPoint3d Vec, GlbPoint3d &Vec_dst);
//���������ĵ��
GLBROT_API float glbDotMul(GlbPoint3d Vec1, GlbPoint3d Vec2);
//���������Ĳ��
GLBROT_API void glbCrossMul(GlbPoint3d Vec1, GlbPoint3d Vec2, GlbPoint3d &VecDST);
//������ת��������
//��һ�������ǽ�Ҫ������ת���ڶ���������ԭʼ����ת���󣬵�������������ת��ľ���
//Rotation �� Origin = destination
GLBROT_API void glbRotmatMul(GlbRotmat Rotation, GlbRotmat Origin, GlbRotmat &destination);

//��γ���� ת��Ϊ ֱ������
GLBROT_API void glbPointGeo2PointRect(GlbPointGeo pGeo, GlbPoint3d &pRect);
//ֱ������ ת��Ϊ ��γ����
GLBROT_API void glbPointRect2PointGeo(GlbPoint3d pRect, GlbPointGeo &pGeo);

//�����ϵľ�γ���� ת��Ϊ ��Ļ�ϵľ�γ����
GLBROT_API void glbGlobePoint2ScreenPoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);
//��Ļ�ϵľ�γ���� ת��Ϊ �����ϵľ�γ����
GLBROT_API void glbScreenPoint2GlobePoint(GlbPoint3d p1, GlbRotmat r, GlbPoint3d &p2);

//ֱ������ ת��Ϊ Բ������
GLBROT_API void glbPointRect2PointRound(GlbPoint3d p3, GlbPointRound &p2, GlbCalib calib);
//Բ������ ת��Ϊ ֱ������
GLBROT_API void glbPointRound2PointRect(GlbPointRound p2, GlbPoint3d &p3, GlbCalib calib);

//���ɳ�ʼ��ת����
GLBROT_API void glbCreateGlbRotmat(GlbRotmat &r);
//������ת����
GLBROT_API void glbCloneGlbRotmat(GlbRotmat r, GlbRotmat &r_dst);

//������Ļ�ϵĵ�p1��p2����ת�Ƕ�
GLBROT_API float glbAngleBetweenPoints(GlbPoint3d p1, GlbPoint3d p2);
//������Ļ�ϵĵ�p1��p2����ת��
GLBROT_API void glbPivotBetweenPoints(GlbPoint3d p1, GlbPoint3d p2, GlbPivot &pivot);

//ŷ���� ת��Ϊ ��ת����
GLBROT_API void glbEularAngle2Rotmat(GlbEularAngle angle, GlbRotmat &r);
//��ת���� ת��Ϊ ��ת����
GLBROT_API void glbRotVector2RotMat(GlbPoint3d angle, GlbRotmat &r);
//��Ļ�ϵĵ�p1�ƶ���p2����ʾ����ת����
GLBROT_API void glbMovingPoints2RotMat(GlbPoint3d p1, GlbPoint3d p2, GlbRotmat &r);
//��ת���ת������ʾ����ת����
GLBROT_API void glbAnglePivot2RotMat(GlbPivot pivot, float angle, GlbRotmat &r);
//��p����pivot����תangle�Ƕ�
GLBROT_API void glbPivotingPoint(GlbPoint3d p, GlbPivot pivot, float angle, GlbPoint3d &p_out);
//������ͼʱ�����������������ת��
GLBROT_API void glbCreateNormPivot(GlbPoint3d p, GlbPoint3d directPoint, bool bHeadDirect, GlbPivot &pivot_h, GlbPivot &pivot_v);//����p�㵽headPoint����ת�ᣬ�Լ��������������ת��

//��ini�ж�ȡ���������Ϣ
GLBROT_API void glbInitDistort(GlbCalib &calib, char* calibFileName);
//���۾�ͷ����������(�������Բ������ת�䵽��Ӧ��ͷ��ʾ��Բ�����꣩
GLBROT_API float glbDistortRadius(float Radius, GlbCalib calib);
//����������任
GLBROT_API float glbUnDistortRadius(float Radius, GlbCalib calib);

//int GetTopLayer(GlbPoint2d pTouch);
