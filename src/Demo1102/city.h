#pragma once

#include <vector>
using std::vector;

class CCity
{
public:
	CCity(void);
	~CCity(void);
	int id;
	char* name;
	char* displayname;
	char* country;
	bool displaycountry; //true: ��ʾ�������ڹ��ҵ���Ϣ��false: ��ʾ���е���Ϣ
	
	char* folder; //����ͼ���ļ���
	char* imgPath; //����ͼƬ�����·��
	vector<vector<char*>> images;



	float latitude; //[-90, 90]
	float longitude; //[-180, 180]
	float timezone; //[-12, 12]

	char* getLocalTimeString(); //���ص��ص�ʱ��
	static char* getTimezoneDiffString(CCity city1, CCity city2); //������������֮���ʱ��
	static char* getTimezoneDiffString(CCity city1); //������������֮���ʱ� ���ֻ��һ���������򷵻ظó����뱱����ʱ���

	static vector<CCity> getCities();
	static bool updateXml();
};