#pragma once
#include "global.h"


//mode4 �����
class CMode4
{
public:
	GlbImage m_icon;
    GlbImage cityView;
	GlbRotmat *m_pGlobeRotMat;
	GlbWindow *m_pWindow;
	vector<CCity> m_cities;
	bool m_bClosed;
    vector<GlbPointGeo> polygon;
	bool m_bIntersect; //��������ཻ
    float length;//������ܳ�
    GlbImage icon_close;//�ر�ͼ��
public:
	CMode4(GlbRotmat *pGlobeRotMat, GlbWindow *pWindow)
	{
		m_cities = CCity::getCities();
		m_icon = glbLoadImage("image\\icon.png");
        icon_close = glbLoadImage("image\\close.png");
		m_pGlobeRotMat = pGlobeRotMat;
		m_pWindow = pWindow;
        m_bClosed = false;
		m_bIntersect = false;
	}
    ~CMode4()
    {
        glbReleaseImage(&icon_close);
        glbReleaseImage(&m_icon);
        glbReleaseImage(&cityView);
    }
    void draw()
	{
		vector<CCity>::iterator it;
		int layer = LAYER_CITY_ICON_START;
		for(it=m_cities.begin(); it!=m_cities.end(); it++)
		{
			GlbPointGeo p1(it->latitude, it->longitude);
			GlbPointGeo p2(90,0);
			glbDrawTexture(m_icon, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, true, true, 5, 5, layer++, GLB_TEX_RECT);
		}

        //�������
        length = 6371.0f / 180.0f * 3.14f * glbDrawPolygon(polygon, true, m_bClosed, *m_pGlobeRotMat, m_pWindow->m_calib, LAYER_LINES);
        
        if(polygon.size()>0)
        {
            glbDrawCircle(polygon[0], true, 3, *m_pGlobeRotMat, m_pWindow->m_calib, LAYER_LINES);
        }
		if(polygon.size()>1)
		{
			GlbPointGeo p1,p2(90,0);
			if(m_bClosed)
			{
				p1 = polygon[0];
			}
			else
			{
				p1 = polygon[polygon.size()-1];
			}
            if(m_bClosed)
            {
                glbDrawTexture(cityView, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, false, true, 50, 24, LAYER_CITY_DETAIL, GLB_TEX_RECT);
                GlbPointGeo texPointGeo10,texPointGeo11;
                GlbPointTex texPoint(0.95,0.05);
                glbPointTex2PointGeo(*m_pGlobeRotMat, p1, true, p2, false, true, 50, 24, GLB_TEX_RECT, texPoint, false, texPointGeo10);
                texPoint.m_x = 0.95, texPoint.m_y = 0.95;
                glbPointTex2PointGeo(*m_pGlobeRotMat, p1, true, p2, false, true, 50, 24, GLB_TEX_RECT, texPoint, false, texPointGeo11);
                //���ر�ͼ��
                glbDrawTexture(icon_close, *m_pGlobeRotMat, m_pWindow->m_calib, texPointGeo10, false, texPointGeo11, false, true, 5, 5, LAYER_CITY_DETAIL_CLOSE, GLB_TEX_RECT);

            }
            else
            {
			    glbDrawTexture(cityView, *m_pGlobeRotMat, m_pWindow->m_calib, p1, true, p2, false, true, 25, 12, LAYER_CITY_DETAIL, GLB_TEX_RECT);
                GlbPointGeo texPointGeo10,texPointGeo11;
                GlbPointTex texPoint(0.9,0.1);
                glbPointTex2PointGeo(*m_pGlobeRotMat, p1, true, p2, false, true, 25, 12, GLB_TEX_RECT, texPoint, false, texPointGeo10);
                texPoint.m_x = 0.9, texPoint.m_y = 0.9;
                glbPointTex2PointGeo(*m_pGlobeRotMat, p1, true, p2, false, true, 25, 12, GLB_TEX_RECT, texPoint, false, texPointGeo11);
                //���ر�ͼ��
                glbDrawTexture(icon_close, *m_pGlobeRotMat, m_pWindow->m_calib, texPointGeo10, false, texPointGeo11, false, true, 5, 5, LAYER_CITY_DETAIL_CLOSE, GLB_TEX_RECT);

            }

		}
	}
	void reset()
	{
        polygon.clear();
        m_bClosed = false;
		m_bIntersect = false;
        glbReleaseImage(&cityView);
	}
    void onClick(int layer, GlbPoint2d point2d_screen)
    {
        if(layer == LAYER_CITY_DETAIL_CLOSE)
        {
            reset();
        }
        else if( (layer <= LAYER_LINES || layer == LAYER_CITY_DETAIL) && !m_bClosed)
        {
            GlbPoint3d point3d_screen, point3d_globe;
			glbPointRound2PointRect(point2d_screen, point3d_screen, m_pWindow->m_calib);

            glbScreenPoint2GlobePoint(point3d_screen, *m_pGlobeRotMat, point3d_globe);
            GlbPointGeo pointGeo_globe;
		    glbPointRect2PointGeo(point3d_globe, pointGeo_globe);
			
			polygon.push_back(pointGeo_globe);//����һ���µ�


			char msg[64];
			//�����ܳ�
            length = 6371.0f / 180.0f * 3.14f * glbDrawPolygon(polygon, true, m_bClosed, *m_pGlobeRotMat, m_pWindow->m_calib, LAYER_LINES);

			if(polygon.size()>2)
			{
				GlbPointGeo A,B,C;
				A = *(polygon.end()-3);
				B = *(polygon.end()-2);
				C = *(polygon.end()-1);
				float angle = glbAngleABC(A,B,C);
				printf("ת��Ϊ%f��\n", angle);
			}

			//�ж��Ƿ�պ�
            if(polygon.size()>1)
            {
                GlbPoint3d startPoint3d;
                glbPointGeo2PointRect(polygon[0], startPoint3d);
                if( glbAngleBetweenPoints(startPoint3d, point3d_globe) < 3 )//�ǵ������һ�������Χ
                {
                    //����αպ���
					polygon.pop_back();

					//�ж���������߶��Ƿ��֮ǰ���ཻ
					m_bIntersect |= polygonIntersect_lastline(polygon, true);

                    m_bClosed = true;
					float omiga = glbGetSteradian(polygon);
					if(omiga > 2*PI)
					{
						omiga = 4*PI - omiga;
					}
					float area = omiga * 6371.0f* 6371.0f;
					if(m_bIntersect)
					{
						sprintf(msg, "����β��������ཻ��$�޷��������\n");
					}
					else
					{
						sprintf(msg, "����ε����Ϊ$%d��ƽ������$���ܳ�Ϊ%d����", (int)(area/10000), (int)length);
					}
                }
                else//�����û�бպ�
                {
                    //�ж��¼�����߶��Ƿ��֮ǰ���ཻ
                    m_bIntersect |= polygonIntersect_lastline(polygon, false);
                    sprintf(msg, "����Ϊ$%d����\n", (int)(length));
                }
                
			    if (txt2ImgHelper(4, msg))//4Ϊģʽ
			    {
				    cityView = glbLoadImage( "temp.png" );
			    }
			    else
			    {
				    cityView = glbLoadImage( "error.png" );
			    }
            }

            //m_bShowDetail = true;
        }
		else
		{

		}
        printf("%d\n", m_bIntersect);
    }
	bool polygonIntersect_lastline(vector<GlbPointGeo> polygon, bool closed)
	{
		int nVertex = polygon.size();
		if(nVertex < 3)
		{
			return false;
		}
		
		GlbPointGeo A,B,C,D;
		if(closed)
		{
			A = polygon[nVertex-1];//AΪ���һ������
			B = polygon[0];//BΪ��һ������
		}
		else
		{
			A = polygon[nVertex-2];//AΪ�����ڶ�������
			B = polygon[nVertex-1];//BΪ���һ������
		}

		for(int i=0; i<(nVertex-2+(int)closed); i++)
		{
			//����polygon�е��߶�
			C = polygon[i];
			D = polygon[i+1];

			if(glbArcsIntersect(A,B,C,D))
			{
				//����һ���ཻ���򷵻�true
                //printf("�ཻ��\n");
				return true;
			}
		}
		//û���ཻ������false
		return false;
	}
};