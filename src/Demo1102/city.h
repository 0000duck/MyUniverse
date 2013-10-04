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
	char* path;
	float latitude; //[-90, 90]
	float longitude; //[-180, 180]
	float timezone; //[-12, 12]
	int population;
	char* description;

	static vector<CCity> getCities();
};

