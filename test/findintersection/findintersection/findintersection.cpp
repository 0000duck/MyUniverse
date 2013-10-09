// http://geospatialmethods.org/spheres/GCIntersect.html#GCIGC
//

#include "stdafx.h"
#include <math.h>

/*
Given two planes (that contain the origin) and a sphere:

(y0z1- y1z0)x - (x0z1- x1z0)y + (x0y1- x1y0)z = 0
(y2z3 - y3z2 )x - (x2z3 - x3z2 )y + (x2y3 - x3y2)z = 0
x2+ y2+ z2= r2

Solve for (x, y, z)
*/

const static double PI = 3.1415927;

//�Ѿ�γ������ת����ֱ�����꣬ lat��Χ��[-90, +90], lng��Χ��[-180��+180]
void getxyz(double lat, double lng, double* x, double* y, double* z)
{
	double phi0 = PI/2 - lat * PI / 180; //������ĵ����ߺͱ�������Z�ᣩ�ļн�
	double lambda0 = lng * PI /180; //������ĵ������ڳ�����ϵ�ͶӰ��X��ļн�
	*x = sin(phi0)*cos(lambda0);
	*y = sin(phi0)*sin(lambda0);
	*z = cos(phi0);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//let R = 1; //�뾶Ϊ1����

	//�ȶ�����A�������˵�(40N��120E��40S��120E)
	double x0, y0, z0;
	getxyz(40, 120, &x0, &y0, &z0);
	double x1, y1, z1;
	getxyz(-40, 120, &x1, &y1, &z1);

	//�ȶ�����B�������˵�(20N��110E��20N��130E)
	double x2, y2, z2;
	getxyz(20, 110, &x2, &y2, &z2);
	double x3, y3, z3;
	getxyz(20, 130, &x3, &y3, &z3);

	//�����ߵĽ��㣨Ӧ����20N��120E��������Ϊ����20N��110E��20N��130E��20Nγ���߲���Great Circle��
	double a = (y0*z1 - y1*z0 );
	double b = (x1*z0 - x0*z1 );
	double c = (x0*y1 - x1*y0 );
	double d = (y2*z3 - y3*z2 );
	double e = (x3*z2 - x2*z3 );
	double f = (x2*y3 - x3*y2 );

	double h = (d*c - f*a)/(e*a - d*b);
	double g = (-b*h-c)/a;
	double k = sqrt(1/(g*g+h*h+1));

	//����1
	double latC1 = asin(k) * 180/PI; //20.28N
	double lngC1 = atan2(h, g)* 180/PI;//120E

	//����2
	double latC2 = asin(-k)* 180/PI; //20.28S
	double lngC2 = atan2(-h, -g)* 180/PI;//60W

	//���жϽ���1����A�������˵�֮��ļн��Ƿ�Ϊ�۽ǣ���Ϊ�۽��򽻵�1����A�ϡ�
	//ͬ���жϳ�����1Ҳ����B�ϡ��ó���A����B���뽻��1�Ľ��ۡ�

	//����2������������
	return 0;
}

