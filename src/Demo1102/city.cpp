#include <afxwin.h>
#include "city.h"
#include "libxml.h"
#include "libxml\xpath.h"

#pragma comment (lib, "libxml2.lib")
#pragma comment (lib, "iconv.lib")

CCity::CCity(void)
{
}

CCity::~CCity(void)
{
}

CString	GetCurAppFilePath() 
{
	TCHAR szPath[MAX_PATH] ;
	GetModuleFileName(NULL, szPath, MAX_PATH) ;
	PathRemoveFileSpec(szPath) ;
	return	szPath ;
}

//http://stackoverflow.com/questions/8308236/performing-arithmetic-on-systemtime
SYSTEMTIME add( SYSTEMTIME s, double seconds ) {

    FILETIME f;
    SystemTimeToFileTime( &s, &f );

    ULARGE_INTEGER u  ; 
    memcpy( &u  , &f , sizeof( u ) );

    const double c_dSecondsPer100nsInterval = 100. * 1.E-9;
    u.QuadPart += seconds / c_dSecondsPer100nsInterval; 

    memcpy( &f, &u, sizeof( f ) );

    FileTimeToSystemTime( &f, &s );
    return s;
 }

char* CCity::getLocalTimeString()
{
	SYSTEMTIME st = {0};
	GetSystemTime(&st); //UTC time

	// Convert to local time provided timezone information.
	SYSTEMTIME localTime = add(st, timezone*60*60);
	char* csLocalTime = new char[32];
	sprintf(csLocalTime, "%02d:%02d", localTime.wHour,
							 localTime.wMinute/*,localTime.wSecond*/);
	return csLocalTime;
}

char* CCity::getTimezoneDiffString(CCity city1, CCity city2)
{
	char* timeDiffString = new char[64];
	if (city2.timezone > city1.timezone)
		sprintf(timeDiffString, "%s��ʱ���%s��%2.1fСʱ", city2.displayname, city1.displayname, abs(city2.timezone - city1.timezone));
	else if (city2.timezone < city1.timezone) 
		sprintf(timeDiffString, "%s��ʱ���%s��%2.1fСʱ", city2.displayname, city1.displayname, abs(city2.timezone - city1.timezone));
	else  //float point comparison?
		sprintf(timeDiffString, "%s��%s����ͬһ��ʱ��", city2.displayname, city1.displayname);
	return timeDiffString;
}

char* CCity::getTimezoneDiffString(CCity city1)
{
	char* timeDiffString = new char[64];
	sprintf(timeDiffString, "%s�ͱ�����ʱ��Ϊ%2.1fСʱ", city1.displayname, city1.timezone - 8);
	return timeDiffString;
}

int code_convert(char* from_charset, char* to_charset, char* inbuf,
               int inlen, char* outbuf, int outlen)
{
    iconv_t cd;
    char** pin = &inbuf;  
    char** pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);  
    if(cd == 0)
       return -1;
 
    memset(outbuf,0,outlen);  
 
    if(iconv(cd,(const char**)pin,(unsigned int *)&inlen,pout,(unsigned int*)&outlen) == -1)
       return -1;  
 
    iconv_close(cd);
 
    return 0;  
}

char* u2g(char *inbuf)  
{
	if (strlen(inbuf) == 0)
	{
		return "";
	}

    int nOutLen = 2 * strlen(inbuf) - 1;
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("utf-8","gb2312",inbuf,strlen(inbuf),szOut,nOutLen))
    {
       free(szOut);
       szOut = NULL;
    }
 
    return szOut;
}  

bool CCity::updateXml()
{
	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = NULL;
	sei.lpVerb = TEXT("runas"); //�Թ���Ա������У����XML�ļ�����C:\Program Files�£� ��Ҫ�Թ���Ա���в����޸��ļ�����ȷ�������ǰ���ҪдȨ�޵��ļ��Ƶ�AppData����ProgramDataĿ¼����ȥ��	
	CString file = GetCurAppFilePath() + TEXT("\\WeatherForecastHelper.exe");
	sei.lpFile = file;//ʹ��ȫ·��
	//sei.lpParameters = TEXT("");
	sei.nShow = SW_SHOWNORMAL;
	if (!ShellExecuteEx(&sei))
	{
		DWORD dwStatus = GetLastError();
		if (dwStatus == ERROR_CANCELLED)
		{
			printf("user cancelled the elevated request!");
		}
		else if (dwStatus == ERROR_FILE_NOT_FOUND)
		{
			printf("File not found!");			
		}
		return false;
	}
	else
	{
		WaitForSingleObject(sei.hProcess,INFINITE);
		DWORD dwRet;
		GetExitCodeProcess(sei.hProcess, &dwRet);
		if (dwRet == 0)
			return true;
		else
		{
			printf("Failed to update xml, please check log.");
			return false;
		}
	}
}

vector<CCity> populatetestdata()
{
	//kennyzx test
	vector<CCity> cities;
	CCity city1;
	city1.id = 0;
	city1.name="����";
	city1.country = "�й�";
	city1.displayname = "����";
	city1.latitude  = 40;
	city1.longitude = 116;
	
	city1.timezone = 8.0f;
	city1.folder = "image\\cities\\�й�&����";
	city1.imgPath = "image\\cities\\�й�&����\\city.jpg";
	vector<char*> v1;
	v1.push_back("�ڽ�");
	v1.push_back("image\\cities\\�й�&����\\�ڽ�\\1.jpg");
	v1.push_back("image\\cities\\�й�&����\\�ڽ�\\2.jpg");
	v1.push_back("image\\cities\\�й�&����\\�ڽ�\\3.jpg");
	vector<char*> v2;
	v2.push_back("����");
	v2.push_back("image\\cities\\�й�&����\\����\\1.jpg");
	v2.push_back("image\\cities\\�й�&����\\����\\2.jpg");
	v2.push_back("image\\cities\\�й�&����\\����\\3.jpg");
	city1.images.push_back(v1);
	city1.images.push_back(v2);

	CCity city2;
	city2.id = 1;
	city2.name="����";
	city2.country = "�ձ�";
	city2.displayname = "����";
	city2.latitude  = 36;
	city2.longitude = 140;

	city2.timezone = 9.0f;
	city2.folder = "image\\cities\\�ձ�&����";
	city2.imgPath = "image\\cities\\�ձ�&����\\city.jpg";
	vector<char*> v3;
	v3.push_back("�ڽ�");
	v3.push_back("image\\cities\\�ձ�&����\\�ڽ�\\1.jpg");
	v3.push_back("image\\cities\\�ձ�&����\\�ڽ�\\2.jpg");
	v3.push_back("image\\cities\\�ձ�&����\\�ڽ�\\3.jpg");
	vector<char*> v4;
	v4.push_back("����");
	v4.push_back("image\\cities\\�ձ�&����\\����\\1.jpg");
	v4.push_back("image\\cities\\�ձ�&����\\����\\2.jpg");
	v4.push_back("image\\cities\\�ձ�&����\\����\\3.jpg");
	vector<char*> v5;
	v5.push_back("����");
	v5.push_back("image\\cities\\�ձ�&����\\����\\��������Ȫ���ձ�⨺�.jpg");
	city2.images.push_back(v3);
	city2.images.push_back(v4);
	city2.images.push_back(v5);

	cities.push_back(city1);
	cities.push_back(city2);
	return cities;
}

vector<CCity> CCity::getCities()
{
	return populatetestdata();

	vector<CCity> cities;
	//
	xmlInitParser();
    xmlDocPtr doc = xmlParseFile("�����Ļ�.xml");
	if (doc)
	{
		xmlXPathContext *xpathCtx = xmlXPathNewContext( doc );
		xmlXPathObject * xpathObj =
			xmlXPathEvalExpression( (xmlChar*)"/cities/city", xpathCtx );
		xmlNode *node = xpathObj->nodesetval->nodeTab[0];
		while (node)
		{
			if (!xmlStrcmp(node->name, (const xmlChar *)"text")) //deal with white space between nodes
			{
				node = node->next;
				continue;
			}

			CCity city;
			xmlAttr *attr = node->properties;
			while ( attr )
			{
				char* val = (char*)(attr->children->content);

				if ((!xmlStrcmp(attr->name, (const xmlChar *)"id"))){
					city.id = atoi(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"name"))){
					city.name = u2g(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"displayname"))){
					city.displayname = u2g(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"country"))){
					city.country = u2g(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"displaycountry"))){
					city.displaycountry = (strcmp(val, "true") == 0);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"latitude"))){
					city.latitude = atof(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"longitude"))){
					city.longitude = atof(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"timezone"))){
					city.timezone = atof(val);
				}
				else if ((!xmlStrcmp(attr->name, (const xmlChar *)"path"))){
					city.folder = u2g(val);
					city.imgPath = u2g(val);
				}
				attr = attr->next;
			}

			cities.push_back(city);
			node = node->next;
		}
		xmlFreeDoc(doc);
	}
	return cities;
}