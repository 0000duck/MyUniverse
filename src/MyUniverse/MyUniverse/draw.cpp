#include "stdafx.h"
#include "draw.h"
#include <math.h>
#include <windows.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
//#include "timer.h"

GlbRotmat g_GlobeRotMat;


void DrawLineOnGlobe(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer)
{
	GlbPoint3d rectStartPoint, rectEndPoint;
	Geo2Rect(geoStartPoint, rectStartPoint);
	Geo2Rect(geoEndPoint, rectEndPoint);
	Sphere2Sphere(rectStartPoint, g_GlobeRotMat, rectStartPoint);
	Sphere2Sphere(rectEndPoint, g_GlobeRotMat, rectEndPoint);
	GlbPointGeo geoStartPoint2, geoEndPoint2;
	Rect2Geo(rectStartPoint, geoStartPoint2);
	Rect2Geo(rectEndPoint, geoEndPoint2);
	DrawLineOnScreen(geoStartPoint2, geoEndPoint2, layer);
}

void DrawLineOnScreen(GlbPointGeo geoStartPoint, GlbPointGeo geoEndPoint, int layer)
{
	GlbPoint3d rectStartPoint, rectEndPoint;
	Geo2Rect(geoStartPoint, rectStartPoint);
	Geo2Rect(geoEndPoint, rectEndPoint);

	float angle = Point2Angle(rectStartPoint, rectEndPoint);
	GlbPoint3d pivot;
	Point2Pivot(rectStartPoint, rectEndPoint, pivot, true);

	int nStep = (int)(angle/FACET_SCACLE_IN_ANGLE)+1;
	float mAngle = angle/nStep;

	glLoadIdentity();											// ���õ�ǰ��ģ�͹۲����
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1.0f,1.0f,0.0f);
	glLineWidth(0.5);//���ò��պ����ߴ�С  

	//�����
	glEnable(GL_LINE_SMOOTH);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);

	for(int i=0; i<nStep; i++)
	{
		GlbPoint3d p3d1,p3d2;
		GlbPoint2d p2d1,p2d2;
		PivotRotPoint(rectStartPoint, pivot, mAngle*i, p3d1);
		Sphere2Round(p3d1, 1.0f, p2d1);
		PivotRotPoint(rectStartPoint, pivot, mAngle*(i+1), p3d2);
		Sphere2Round(p3d2, 1.0f, p2d2);
		if(fabs(p2d1.m_x-p2d2.m_x)<MAX_FACET_SHOW_THRESHOLD && fabs(p2d1.m_y-p2d2.m_y)<MAX_FACET_SHOW_THRESHOLD)
		{
			glBegin(GL_LINES);//�����պ����� 
			glVertex3f(p2d1.m_x,p2d1.m_y, layer);
			glVertex3f(p2d2.m_x,p2d2.m_y, layer);
			glEnd();
		}
	}
}

void DrawGlobe(GLuint texture)
{
	GlbPoint3d pClose;
	GlbPointGeo pGeo,pGeoDirect;
	pGeo.m_lat = 0;
	pGeo.m_lng = 0;
	pGeoDirect.m_lat = 90;
	pGeoDirect.m_lng = 0;

	DrawTexture(
		texture,	//���ʵı��
		pGeo,			//��ͼ�����ĵ�
		true,	//���ĵ��������� globe����ϵ(true) or screen����ϵ(false)
		pGeoDirect,	//��ͼ����Ĳο���
		true,		//����ο���ʵ�� globe����ϵ(true) or screen����ϵ(false)
		true,		//ͼƬ����ο���(ture) or ����ο���(false)
		360,			//��ͼ�Ŀ��(��λ:�Ƕ�)
		180,			//��ͼ�ĸ߶�(��λ:�Ƕ�)
		0,			//��ͼ���ڵĲ�
		GLB_TEX_BELT,
		pClose		//������ͼ���Ͻǵ�����
		);
}

void DrawTexture(
				 GLuint TextureName,	//���ʵı��
				 GlbPointGeo pGeo,		//��ͼ�����ĵ�
				 bool bPointOnGlobe,	//���ĵ��������� globe����ϵ(true) or screen����ϵ(false)
				 GlbPointGeo pGeoDirect,//��ͼ����Ĳο���
				 bool bDirOnGlobe,		//����ο���ʵ�� globe����ϵ(true) or screen����ϵ(false)
				 bool bHeadDirect,		//ͼƬ����ο���(ture) or ����ο���(false)
				 float width,			//��ͼ�Ŀ��(��λ:�Ƕ�)
				 float height,			//��ͼ�ĸ߶�(��λ:�Ƕ�)
				 float layer,			//��ͼ���ڵĲ�
				 GlbTexMode mode,
				 GlbPoint3d &pClose		//������ͼ���Ͻǵ�����
				 )
{
	int nRow = (int)(height/FACET_SCACLE_IN_ANGLE)+1;
	int nCol = (int)(width/FACET_SCACLE_IN_ANGLE)+1;


	float mHeight = height/nRow;
	float mWidth = width/nCol;

	//���㵱ǰ���е�λ��
	GlbPoint3d pRect;
	GlbPoint2d r_pRound;
	Geo2Rect(pGeo, pRect);
	if(bPointOnGlobe)
	{
		Sphere2Sphere(pRect, g_GlobeRotMat, pRect);
	}

	GlbPoint3d pivot_v, pivot_h;
	GlbPoint3d pRectDirect;
	Geo2Rect(pGeoDirect, pRectDirect);
	if(bDirOnGlobe)
	{
		Sphere2Sphere(pRectDirect, g_GlobeRotMat, pRectDirect);
	}

	CreateNormPivot( pRect, pRectDirect, bHeadDirect, pivot_h, pivot_v);
	if(mode == GLB_TEX_BELT)
	{
		pivot_h = pRectDirect;
	}

	GlbPoint3d **PointArr = new GlbPoint3d*[nRow+1];
	for(int x=0; x<=nRow; x++)
	{
		PointArr[x] = new GlbPoint3d[nCol+1];
	}

	float angle_h0 = -width/2.0f;
	float angle_v0 = -height/2.0f;
	glPointSize(1.0f);

	for(int x=0; x<=nCol; x++)
	{
		for(int y=0; y<=nRow; y++)
		{
			float angle_h = angle_h0 + x * mWidth;
			float angle_v = angle_v0 + y * mHeight;

			GlbPoint3d p_img;
			PivotRotPoint(pRect, pivot_v, angle_v, p_img);
			PivotRotPoint(p_img, pivot_h, angle_h, p_img);
			Sphere2Round(p_img, 1.0f, r_pRound);

			PointArr[y][x].m_x = r_pRound.m_x;
			PointArr[y][x].m_y = r_pRound.m_y;
		}
	}

	glLoadIdentity();	
	glColor3f(1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, TextureName);	

	float mRectWidth = 1.0f/nCol;
	float mRectHeight = 1.0f/nRow;

	for(int x=0; x<nCol; x++)
	{
		for(int y=0; y<nRow; y++)
		{
			float th = 0.4f;

			GlbPoint2d pRound[4];
			pRound[0].m_x = PointArr[y][x].m_x;
			pRound[0].m_y = PointArr[y][x].m_y;
			pRound[1].m_x = PointArr[y][x+1].m_x;
			pRound[1].m_y = PointArr[y][x+1].m_y;
			pRound[2].m_x = PointArr[y+1][x+1].m_x;
			pRound[2].m_y = PointArr[y+1][x+1].m_y;
			pRound[3].m_x = PointArr[y+1][x].m_x;
			pRound[3].m_y = PointArr[y+1][x].m_y;


			if( fabs(pRound[0].m_x-pRound[1].m_x) < th &&
				fabs(pRound[0].m_x-pRound[2].m_x) < th &&
				fabs(pRound[1].m_x-pRound[3].m_x) < th &&
				fabs(pRound[2].m_x-pRound[3].m_x) < th &&

				fabs(pRound[0].m_y-pRound[1].m_y) < th &&
				fabs(pRound[0].m_y-pRound[2].m_y) < th &&
				fabs(pRound[1].m_y-pRound[3].m_y) < th &&
				fabs(pRound[2].m_y-pRound[3].m_y) < th)
			{
				glBegin(GL_QUADS);						// ����������
				
				glTexCoord2f((x) * mRectWidth, (y) * mRectHeight); 
				glVertex3f(pRound[0].m_x, pRound[0].m_y, layer);				// ����

				glTexCoord2f((x+1) * mRectWidth, (y) * mRectHeight); 
				glVertex3f(pRound[1].m_x, pRound[1].m_y, layer);				// ����
				glTexCoord2f((x+1) * mRectWidth, (y+1) * mRectHeight); 
				glVertex3f(pRound[2].m_x, pRound[2].m_y, layer);				// ����

				
				glTexCoord2f((x) * mRectWidth, (y+1) * mRectHeight); 
				glVertex3f(pRound[3].m_x, pRound[3].m_y, layer);				// ����
				glEnd();
			}
		}
	}


	for(int x=0; x<=nRow; x++)
	{
		delete [] PointArr[x];
	}
	delete [] PointArr;

	//���pClose
	PivotRotPoint(pRect, pivot_v, height/2.0f, pClose);
	PivotRotPoint(pClose, pivot_h, width/2.0f, pClose);
}


void DrawBelt( GLuint TextureName, 
			  GlbPointGeo pGeo, 
			  bool bPointOnGlobe, 
			  GlbPointGeo pGeoDirect, 
			  bool bDirOnGlobe, 
			  bool bHeadDirect, 
			  float width, 
			  float height, 
			  float layer )
{
	GlbPoint3d pClose;
	DrawTexture(
		TextureName,	//���ʵı��
		pGeo,			//��ͼ�����ĵ�
		bPointOnGlobe,	//���ĵ��������� globe����ϵ(true) or screen����ϵ(false)
		pGeoDirect,	//��ͼ����Ĳο���
		bDirOnGlobe,		//����ο���ʵ�� globe����ϵ(true) or screen����ϵ(false)
		bHeadDirect,		//ͼƬ����ο���(ture) or ����ο���(false)
		width,			//��ͼ�Ŀ��(��λ:�Ƕ�)
		height,			//��ͼ�ĸ߶�(��λ:�Ƕ�)
		layer,			//��ͼ���ڵĲ�
		GLB_TEX_BELT,
		pClose		//������ͼ���Ͻǵ�����
		);
}
