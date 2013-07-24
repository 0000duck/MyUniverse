
// MyUniverseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyUniverse.h"
#include "MyUniverseDlg.h"
#include "afxdialogex.h"
#include "GlobeThread.h"
#include "udpThread.h"
#include "libxml.h"
#include "opencv.hpp"
#include "Ktmfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CRITICAL_SECTION g_StoryPage_CS;
extern CStoryPage g_StoryPage;

extern CRITICAL_SECTION g_GlobeRotMat_CS;
extern GlbRotmat g_GlobeRotMat;
extern GlbEularAngle g_GlobeEularAngle;

int g_bMainThreadActive;
int g_bGlbThreadActive;
int g_bTimingThreadActive;
int g_bUdpThreadActive;

#ifdef _DEBUG
#pragma comment( linker, "/subsystem:console /entry:WinMainCRTStartup" )
#endif

char* u2g(char *inbuf);

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    // ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyUniverseDlg �Ի���




CMyUniverseDlg::CMyUniverseDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CMyUniverseDlg::IDD, pParent)
    , m_edit_rotx(0)
    , m_edit_roty(0)
    , m_edit_rotz(0)
    , m_slider_rotx(0)
    , m_slider_roty(0)
    , m_slider_rotz(0)
    , m_story_path(_T(""))
    , m_chapter_value(_T(""))
    , m_page_value(_T(""))
    , m_frame_rate(0)
    , m_rotating(FALSE)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMyUniverseDlg::~CMyUniverseDlg()
{

}

void CMyUniverseDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_m_edit_rotx, m_edit_rotx);
    DDV_MinMaxInt(pDX, m_edit_rotx, 0, 360);
    DDX_Text(pDX, IDC_m_edit_roty, m_edit_roty);
    DDV_MinMaxInt(pDX, m_edit_roty, 0, 360);
    DDX_Text(pDX, IDC_m_edit_rotz, m_edit_rotz);
    DDV_MinMaxInt(pDX, m_edit_rotz, 0, 360);
    DDX_Slider(pDX, IDC_m_slider_rotx, m_slider_rotx);
    DDX_Slider(pDX, IDC_m_slider_roty, m_slider_roty);
    DDX_Slider(pDX, IDC_m_slider_rotz, m_slider_rotz);
    DDX_Text(pDX, IDC_EDIT_SOTRY_PATH, m_story_path);
    DDV_MaxChars(pDX, m_story_path, 512);
    DDX_Control(pDX, IDC_COMBO_CHAPTER, m_chapter_select);
    DDX_CBString(pDX, IDC_COMBO_CHAPTER, m_chapter_value);
    DDV_MaxChars(pDX, m_chapter_value, 512);
    DDX_Control(pDX, IDC_COMBO_PAGE, m_page_select);
    DDX_CBString(pDX, IDC_COMBO_PAGE, m_page_value);
    DDX_Text(pDX, IDC_EDIT_FRAME_RATE, m_frame_rate);
    DDV_MinMaxInt(pDX, m_frame_rate, 0, 100);
    DDX_Check(pDX, IDC_CHECK_AUTO_ROT, m_rotating);
    DDX_Control(pDX, IDC_m_slider_rotz, m_slider_rotz_ctrl);
    DDX_Control(pDX, IDC_m_edit_rotz, m_edit_rotz_ctrl);
    DDX_Control(pDX, IDC_m_slider_rotx, m_slider_rotx_ctrl);
    DDX_Control(pDX, IDC_m_slider_roty, m_slider_roty_ctrl);
}

BEGIN_MESSAGE_MAP(CMyUniverseDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CMyUniverseDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CMyUniverseDlg::OnBnClickedCancel)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_m_slider_rotx, &CMyUniverseDlg::OnNMCustomdrawSliderRotx)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_m_slider_roty, &CMyUniverseDlg::OnNMCustomdrawSliderRoty)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_m_slider_rotz, &CMyUniverseDlg::OnNMCustomdrawSliderRotz)
    ON_EN_CHANGE(IDC_m_edit_rotx, &CMyUniverseDlg::OnEnChangeEditRotx)
    ON_EN_CHANGE(IDC_m_edit_roty, &CMyUniverseDlg::OnEnChangeEditRoty)
    ON_EN_CHANGE(IDC_m_edit_rotz, &CMyUniverseDlg::OnEnChangeEditRotz)
    //ON_EN_CHANGE(IDC_EDIT1, &CMyUniverseDlg::OnEnChangeEdit1)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CMyUniverseDlg::OnBnClickedButtonBrowse)
    ON_CBN_SELCHANGE(IDC_COMBO_CHAPTER, &CMyUniverseDlg::OnCbnSelchangeComboChapter)
    ON_CBN_SELCHANGE(IDC_COMBO_PAGE, &CMyUniverseDlg::OnCbnSelchangeComboPage)
    ON_BN_CLICKED(IDC_BUTTON_STEP, &CMyUniverseDlg::OnBnClickedButtonStep)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &CMyUniverseDlg::OnBnClickedButtonPlay)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CMyUniverseDlg::OnBnClickedButtonPause)
    ON_EN_CHANGE(IDC_EDIT_FRAME_RATE, &CMyUniverseDlg::OnEnChangeEditFrameRate)
    ON_BN_CLICKED(IDC_CHECK_AUTO_ROT, &CMyUniverseDlg::OnBnClickedCheckAutoRot)

    //�Զ�����Ϣ
    ON_MESSAGE(WM_GLB_UPDATEDATA,OnGlbUpdateData)  
    ON_MESSAGE(WM_GLB_UDPREADONEPAGE, OnGlbUdpReadOnePage)
END_MESSAGE_MAP()


// CMyUniverseDlg ��Ϣ�������

BOOL CMyUniverseDlg::OnInitDialog()
{
    InitializeCriticalSection(&g_StoryPage_CS);//��ʼ���ٽ���
    InitializeCriticalSection(&g_GlobeRotMat_CS);//��ʼ���ٽ���
    g_bMainThreadActive = 1; //���߳̿�ʼ�����������~CMyUniverseDlg()������

    CDialogEx::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������

    //��ȡini�ļ��е�����
    char lpStoryPath[512];
    GetPrivateProfileString("MyUniverseCfg", "StoryPath", "", lpStoryPath, 512, ".\\config.ini");
    m_story_path = lpStoryPath;
    UpdateData(0);
    ReadChapterStruct();

    //��ʼ����ת����
    g_GlobeEularAngle.m_1_Horz = 0;
    g_GlobeEularAngle.m_2_Vert = 0;
    g_GlobeEularAngle.m_3_Axis = 0;
    glbEularAngle2Rotmat(g_GlobeEularAngle, g_GlobeRotMat);

    CreateThread(0, 0, GlobeThread, 0,0,0);//����OpenGL��ʾ�߳�
    CreateThread(0, 0, TimingThread, (LPVOID)this,0,0);//����ʱ������߳�
    CreateThread(0, 0, UdpThread, (LPVOID)this,0,0);//����ʱ������߳�

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyUniverseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyUniverseDlg::OnPaint()
{
    //UpdateData(PUT_DATA);
    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyUniverseDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CMyUniverseDlg::OnBnClickedOk()
{
    return;
}


void CMyUniverseDlg::OnBnClickedCancel()
{
    if( 2 == MessageBox("���Ҫ�˳���", NULL, MB_OKCANCEL))
    {
        return;
    }
    else
    {
        g_bMainThreadActive = 0;//���̼߳����˳�����ʼ��OnInitDialog()�����У�

        while( g_bGlbThreadActive || g_bTimingThreadActive)//�ȴ����߳��˳�
        {
            MSG msg;
            PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            Sleep(1);
        }

        CDialogEx::OnCancel();//�����˳�
    }
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotx(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(GET_DATA);
    m_edit_rotx = (int)(m_slider_rotx/100.0*360);
    UpdateData(PUT_DATA);
    GlobeRotate(m_edit_rotx, m_edit_roty, m_edit_rotz, g_GlobeRotMat);
    *pResult = 0;
}


void CMyUniverseDlg::OnNMCustomdrawSliderRoty(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(GET_DATA);
    m_edit_roty = (int)(m_slider_roty/100.0*360);
    UpdateData(PUT_DATA);
    GlobeRotate(m_edit_rotx, m_edit_roty, m_edit_rotz, g_GlobeRotMat);
    *pResult = 0;
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotz(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(GET_DATA);
    m_edit_rotz = (int)(m_slider_rotz/100.0*360);
    UpdateData(PUT_DATA);
    GlobeRotate(m_edit_rotx, m_edit_roty, m_edit_rotz, g_GlobeRotMat);
    *pResult = 0;
}


void CMyUniverseDlg::OnEnChangeEditRotx()
{
    UpdateData(GET_DATA);
    m_slider_rotx = (int)(m_edit_rotx/360.0*100);
    UpdateData(PUT_DATA);
}


void CMyUniverseDlg::OnEnChangeEditRoty()
{
    UpdateData(GET_DATA);
    m_slider_roty = (int)(m_edit_roty/360.0*100);
    UpdateData(PUT_DATA);
}


void CMyUniverseDlg::OnEnChangeEditRotz()
{
    UpdateData(GET_DATA);
    m_slider_rotz = (int)(m_edit_rotz/360.0*100);
    UpdateData(PUT_DATA);
}

void CMyUniverseDlg::OnBnClickedButtonBrowse()
{
    char szPath[MAX_PATH];     //���ѡ���Ŀ¼·��
    CString str;
    ZeroMemory(szPath, sizeof(szPath));  
 
    BROWSEINFO bi;  
    bi.hwndOwner = m_hWnd;  
    bi.pidlRoot = NULL;  
    bi.pszDisplayName = szPath;  
    bi.lpszTitle = "��ѡ��γ�Ŀ¼��";  
    bi.ulFlags = 0;  
    bi.lpfn = NULL;  
    bi.lParam = 0;  
    bi.iImage = 0;  

    //����ѡ��Ŀ¼�Ի���
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);  
    if(lp && SHGetPathFromIDList(lp, szPath))  
    {
       m_story_path = szPath;
    }
    UpdateData(0);
    ReadChapterStruct();
    ::WritePrivateProfileString("MyUniverseCfg","StoryPath",szPath,".\\config.ini");
}

void CMyUniverseDlg::ReadChapterStruct()
{
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

    m_chapter_select.ResetContent();
    if(m_story_path.GetLength() != 0)
    m_story_path += "\\*";

	hFind = FindFirstFile(m_story_path, &FindFileData);
	
	if(hFind == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox ("Invalid file handle.\n");
	}
	else
	{
		do
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
				//printf ("����һ��Ŀ¼\n");
                CString chapterName = FindFileData.cFileName;
                if(chapterName != "." && chapterName != "..")
                {
                    m_chapter_select.AddString(FindFileData.cFileName);
                }
			}
		}while (FindNextFile(hFind, &FindFileData) != 0);

		DWORD dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			//printf ("FindNextFile error. Error is %u\n", dwError);
            //AfxMessageBox("ERROR_NO_MORE_FILES");
		}
	}
    m_chapter_select.SetCurSel(0);
    ReadPageStruct();
}

CString CMyUniverseDlg::GetPageStructPath(CString StoryPath, CString ChapterName)
{
    CString page_struct_path = StoryPath;
    int pathLength = page_struct_path.GetLength();
    char lastChar = *(page_struct_path.GetBuffer()+pathLength-1);
    if(lastChar != '\\')
    page_struct_path +="\\";
    page_struct_path += ChapterName;
    page_struct_path += "\\";
    return page_struct_path;
}

void CMyUniverseDlg::ReadPageStruct()//�δ�֧�֣�folder��dds��jpg��avi��wmv
{
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

    m_page_select.ResetContent();//��ն����˵���ҳ�棩������

    UpdateData(GET_DATA);//�ӽ����ϻ�ȡm_chapter_value

    m_page_struct_path = GetPageStructPath(m_story_path, m_chapter_value);

    CString XML_FilePath = FindXMLFilePath(m_page_struct_path);//�ж��Ƿ���xml�ļ�����

    if(XML_FilePath == "")
    {
	    hFind = FindFirstFile(m_page_struct_path+"*", &FindFileData);
	
	    if(hFind == INVALID_HANDLE_VALUE)
	    {
		    //AfxMessageBox ("Invalid file handle.\n");
	    }
	    else
	    {
		    do
		    {
                CString chapterName = FindFileData.cFileName;
                if(chapterName != "." && chapterName != "..")
                {
                    char* suffix = chapterName.GetBuffer() + chapterName.GetLength() -4;
			        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
				        //printf ("����һ��Ŀ¼\n");
                        m_page_select.AddString(FindFileData.cFileName);
			        }
                    else if( _stricmp(suffix,".jpg") ==0
                          || _stricmp(suffix,".dds") ==0
                          || _stricmp(suffix,".avi") ==0
                          || _stricmp(suffix,".wmv") ==0
                          )
                    {
                        m_page_select.AddString(FindFileData.cFileName);
                    }
                }
		    }while (FindNextFile(hFind, &FindFileData) != 0);

		    DWORD dwError = GetLastError();
		    FindClose(hFind);
		    if (dwError != ERROR_NO_MORE_FILES) 
		    {
			    //printf ("FindNextFile error. Error is %u\n", dwError);
                //AfxMessageBox("ERROR_NO_MORE_FILES");
		    }
	    }
    }
    else//there is one xml file!
    {
        xmlDocPtr doc; //�ļ�ָ��
        xmlNodePtr cur;//�ڵ�ָ��
        doc = xmlParseFile(XML_FilePath);
        cur = xmlDocGetRootElement(doc);
        //AfxMessageBox((char*)cur->name);
        cur = cur->xmlChildrenNode;
        while(cur!=NULL)
        {
            if(xmlHasProp(cur,(xmlChar*)"imageName"))
            {
                xmlChar* szImageName = xmlGetProp(cur,(xmlChar*) "imageName");
                char* imageName_gb2312 = u2g((char*)szImageName);

                m_page_select.AddString(imageName_gb2312);
                
                free(imageName_gb2312);
                xmlFree(szImageName);
            }
            cur = cur->next;
        }
        xmlFreeDoc(doc);
    }

    m_page_select.SetCurSel(0);
    ReadOnePage(true);
}

void CMyUniverseDlg::ReadOnePage(bool bUpdateDataFromUI)
{
    if(bUpdateDataFromUI)
    {
        UpdateData(GET_DATA);//�ӽ����ϻ�ȡm_chapter_value��m_page_value
    }

    m_page_struct_path = GetPageStructPath(m_story_path, m_chapter_value);
    CString XML_FilePath = FindXMLFilePath(m_page_struct_path);//�ж��Ƿ���xml�ļ�����

    
    CString pageFileName;
    if(XML_FilePath=="")
    {
        //��m_page_value��Ϊ�ļ���
        pageFileName = m_page_value;
        //AfxMessageBox(pagePath);
    }
    else
    {
        //��xml�ļ��в����ļ���
        xmlDocPtr doc; //�ļ�ָ��
        xmlNodePtr cur;//�ڵ�ָ��
        doc = xmlParseFile(XML_FilePath);
        cur = xmlDocGetRootElement(doc);
        //AfxMessageBox((char*)cur->name);
        cur = cur->xmlChildrenNode;
        while(cur!=NULL)
        {
            if(xmlHasProp(cur,(xmlChar*)"imageName"))
            {
                xmlChar* szImageName = xmlGetProp(cur,(xmlChar*) "imageName");
                xmlChar* imageName_gb2312 = (xmlChar*)u2g((char*)szImageName);
                xmlChar* imageName_fromDlg = (xmlChar*)(LPSTR)(LPCTSTR)m_page_value;
                if(!xmlStrcmp(imageName_gb2312, imageName_fromDlg))
                {
                    //��ȡbPlaying
                    pageFileName = xmlGetProp(cur,(xmlChar*) "href") + 2;
                }
                free(imageName_gb2312);
                xmlFree(szImageName);
            }
            cur = cur->next;
        }
        xmlFreeDoc(doc);
    }

    CString pagePath = m_page_struct_path + pageFileName;
    char* suffix = pagePath.GetBuffer() + pagePath.GetLength() - 4;

    EnterCriticalSection(&g_StoryPage_CS);

    //���ȫ������
    g_StoryPage.bEmpty = 1;
    g_StoryPage.FrameNames.clear();
    g_StoryPage.bPlaying = 0;
    g_StoryPage.bRotating = 0;
    g_StoryPage.bClockwise = 0;
    g_StoryPage.rotationRate = 2;
    m_rotating = 0;//���ƽ���

    if( _stricmp(suffix,".jpg") == 0)
    {
        //AfxMessageBox("this is a jpg file");
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = JPG;
    }
    else if( _stricmp(suffix,".png") == 0)
    {
        //AfxMessageBox("this is a png file");
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = PNG;
    }
    else if( _stricmp(suffix,".dds") == 0)
    {
        //AfxMessageBox("this is a dds file");
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 0;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = DDS;
    }
    else if( _stricmp(suffix,".avi") == 0)
    {
        //AfxMessageBox("this is a avi file. �ݲ�֧�ִ˸�ʽ");
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 1;
        g_StoryPage.bPlaying = 1;
        g_StoryPage.frameRate = 30;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = AVI;
        g_StoryPage.nCurFrame = 0;
        m_frame_rate = g_StoryPage.frameRate;
    }
    else if( _stricmp(suffix,".wmv") == 0)
    {
        //AfxMessageBox("this is a wmv file. �ݲ�֧�ִ˸�ʽ");
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 1;
        g_StoryPage.bPlaying = 1;
        g_StoryPage.frameRate = 30;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = WMV;
        g_StoryPage.nCurFrame = 0;
        m_frame_rate = g_StoryPage.frameRate;
    }
    else
    {
        //AfxMessageBox("this is a folder");
        g_StoryPage.bEmpty = 0;
        g_StoryPage.bMovie = 1;
        g_StoryPage.pagePath = pagePath;
        g_StoryPage.storyType = FOLDER;
        g_StoryPage.nFrames = 0;	

        ReadFolderContent(pagePath, "dds");
        if(g_StoryPage.nFrames == 0)
        ReadFolderContent(pagePath, "jpg");
        if(g_StoryPage.nFrames == 0)
        ReadFolderContent(pagePath, "png");
    }
    ReadStoryConfigXML();
    LeaveCriticalSection(&g_StoryPage_CS);
}

void CMyUniverseDlg::OnCbnSelchangeComboChapter()
{
    ReadPageStruct();
}


void CMyUniverseDlg::OnCbnSelchangeComboPage()
{
    ReadOnePage(true);
}

void CMyUniverseDlg::ReadFolderContent(CString folderPath, CString suffix)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    CString path = folderPath;
    path += "\\*.";
    path += suffix;
    hFind = FindFirstFile(path, &FindFileData);

	if(hFind == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox ("Invalid file handle.\n");
	}
	else
	{
		do
		{
            CString FrameName = folderPath;
            FrameName += "\\";
            FrameName += FindFileData.cFileName;
            g_StoryPage.FrameNames.push_back(FrameName);
            g_StoryPage.nFrames++;
		}while (FindNextFile(hFind, &FindFileData) != 0);
    }
    g_StoryPage.nCurFrame = 0;
}

void CMyUniverseDlg::GlobeRotate(int Horz, int Vert, int Axis, GlbRotmat &r)
{
    EnterCriticalSection(&g_GlobeRotMat_CS);
    if(fabs(g_GlobeEularAngle.m_1_Horz - Horz)>5)
    {
        g_GlobeEularAngle.m_1_Horz = (float)Horz;
    }
    if(fabs(g_GlobeEularAngle.m_2_Vert - Vert)>5)
    {
        g_GlobeEularAngle.m_2_Vert = (float)Vert;
    }
    if(fabs(g_GlobeEularAngle.m_3_Axis - Axis) > 5)//��Ϊslider��С�ֱ����൱��3.6�ȽǶ�
    {
        g_GlobeEularAngle.m_3_Axis = (float)Axis;
    }
    glbEularAngle2Rotmat(g_GlobeEularAngle, r);
    LeaveCriticalSection(&g_GlobeRotMat_CS);
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
    int nOutLen = 2 * strlen(inbuf) - 1;
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("utf-8","gb2312",inbuf,strlen(inbuf),szOut,nOutLen))
    {
       free(szOut);
       szOut = NULL;
    }
 
    return szOut;
}  

CString CMyUniverseDlg::FindXMLFilePath(CString pageStructPath)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    hFind = FindFirstFile(pageStructPath+"*.xml", &FindFileData);
    if(hFind == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox ("Invalid file handle.\n");
        return "";
	}
	else
	{
        CString XML_FilePath = pageStructPath;
        CString XML_FileName = FindFileData.cFileName;
        XML_FilePath += XML_FileName;
        return XML_FilePath;
    }
}

bool CMyUniverseDlg::Find_ID_in_XML(CString XML_FilePath, int id, CString &imageName, CString &href)
{
    xmlInitParser();
    xmlDocPtr doc;
    doc = xmlParseFile(XML_FilePath); //��ȡ�ļ�

    xmlXPathContextPtr context = xmlXPathNewContext(doc);//����context
    if (context == NULL)
    {  
       printf("context is NULL\n");
       return false;
    }

    xmlXPathRegisterNs(context,(const xmlChar *)"ns", (const xmlChar *)"http://localhost/ImageryDescriptionDocumentFile.xsd");

    CString Xpath = "/ns:ImageryDescription/ns:imageryDescription[@id=";
    char number[10];
    _itoa_s(id, number, 10, 10);
    Xpath += number;
    Xpath += "]";
    //xmlChar* szXpath = (xmlChar*)"/ns:ImageryDescription/ns:imageryDescription[@id=";//0]";

    xmlXPathObjectPtr result = xmlXPathEvalExpression((xmlChar*)(LPSTR)(LPCTSTR)Xpath, context);

    if (result == NULL || xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        printf("can't find element!\n");
    }
    else
    {
        xmlNodePtr pNode = result->nodesetval->nodeTab[0];

        xmlChar* id_char = xmlGetProp(pNode,(xmlChar*) "id");
        int id = atoi((char*)id_char);
        printf("id = %d\n", id);
        xmlChar* href = xmlGetProp(pNode,(xmlChar*) "href");
        printf("href = %s\n", (char*)href);
        xmlChar* imageName = xmlGetProp(pNode,(xmlChar*) "imageName");
        printf("imageName = %s\n", (char*)imageName);
    }



    xmlCleanupParser();

    return true;
}

void CMyUniverseDlg::ReadStoryConfigXML()
{
    //�˺���������libxml2�⣬
    //ʹ�÷����ο���http://www.blogjava.net/wxb_nudt/archive/2007/11/18/161340.html
    //������������ο���http://ling091.iteye.com/blog/295872

    //AfxMessageBox(m_page_struct_path);//���½ڵ�Ŀ¼��
    CString XML_FilePath = FindXMLFilePath(m_page_struct_path);
    if(XML_FilePath == "")
	{
		//AfxMessageBox ("Invalid file handle.\n");
	}
	else
	{
        //AfxMessageBox(XML_FilePath);
        xmlDocPtr doc; //�ļ�ָ��
        xmlNodePtr cur;//�ڵ�ָ��
        doc = xmlParseFile(XML_FilePath);
        cur = xmlDocGetRootElement(doc);
        //AfxMessageBox((char*)cur->name);
        cur = cur->xmlChildrenNode;
        while(cur!=NULL)
        {
            if(xmlHasProp(cur,(xmlChar*)"imageName"))
            {
                xmlChar* szAttr = xmlGetProp(cur,(xmlChar*) "imageName");
                //AfxMessageBox((char*)szAttr);
                xmlChar* imageName_xml = szAttr;
                xmlChar* imageName_xml_gb2312 = (xmlChar*)u2g((char*)imageName_xml);
                xmlChar* imageName_page = (xmlChar*)(LPSTR)(LPCTSTR)m_page_value;
                if(!xmlStrcmp(imageName_xml_gb2312, imageName_page))
                {
                    //AfxMessageBox((char*)filename_xml_gb2312);
                    //��ȷ�����ҵ���Page������

                    //��ȡbPlaying
                    xmlChar* bPlaying_char = xmlGetProp(cur,(xmlChar*) "playing");
                    if(!xmlStrcmp(bPlaying_char, (xmlChar*)"true"))
                    {
                        g_StoryPage.bPlaying = 1;
                    }

                    //��ȡframeRate
                    xmlChar* frameRate_char = xmlGetProp(cur,(xmlChar*) "frameRate");
                    if(frameRate_char)
                    {
                        int frameRate = atoi((char*)frameRate_char);
                        g_StoryPage.frameRate = frameRate;
                        m_frame_rate = frameRate;
                        //UpdateData(PUT_DATA);
                    }

                    //��ȡrotating
                    xmlChar* bRotating_char = xmlGetProp(cur,(xmlChar*) "rotating");
                    if(!xmlStrcmp(bRotating_char, (xmlChar*)"true"))
                    {
                        g_StoryPage.bRotating = 1;
                        m_rotating = 1;
                        //UpdateData(PUT_DATA);
                    }
                    else
                    {
                        g_StoryPage.bRotating = 0;
                        m_rotating = 0;
                        //UpdateData(PUT_DATA);
                    }
                }
                free(imageName_xml_gb2312);
                xmlFree(szAttr);
            }
            cur = cur->next;
        }
        xmlFreeDoc(doc);
    }
    UpdateData(PUT_DATA);
    return;
}

unsigned char* CMyUniverseDlg::convert (unsigned char *in, char *encoding)
{
    unsigned char *out;
    int ret,size,out_size,temp;
    xmlCharEncodingHandlerPtr handler;
    size = (int)strlen((const char *)in)+1;
    out_size = size*2-1;
    out = (unsigned char*)malloc((size_t)out_size);
    if (out) {
        handler = xmlFindCharEncodingHandler(encoding);
        if (!handler) {
            free(out);
            out = NULL;
        }
    }
    if (out) {
        temp=size-1;
        ret = handler->output(out, &out_size, in, &temp);
        if (ret || temp-size+1) {
            if (ret) {
                printf("conversion wasn't successful.\n");
            } else {
                printf("conversion wasn't successful. converted: \n");
              }
                free(out);
                out = NULL;
            } else {
                out = (unsigned char*)realloc(out,out_size+1);
                out[out_size]=0; /*null terminating out*/
            }
        } else {
            printf("no mem\n");
    }
    return (out);
}


void CMyUniverseDlg::OnBnClickedButtonPlay()
{
    g_StoryPage.bPlaying = 1;
}


void CMyUniverseDlg::OnBnClickedButtonPause()
{
    g_StoryPage.bPlaying = 0;
}

void CMyUniverseDlg::OnBnClickedButtonStep()
{
    g_StoryPage.nCurFrame += 1;
    g_StoryPage.nCurFrame %= g_StoryPage.nFrames;
}


void CMyUniverseDlg::OnEnChangeEditFrameRate()
{
    UpdateData(GET_DATA);
    g_StoryPage.frameRate = m_frame_rate;
}


void CMyUniverseDlg::OnBnClickedCheckAutoRot()
{
    UpdateData(GET_DATA);
    g_StoryPage.bRotating = !!m_rotating;
}

LRESULT CMyUniverseDlg::OnGlbUpdateData(WPARAM wParam, LPARAM lParam)  
{  
    //UpdateData(wParam);  
    m_slider_rotx_ctrl.SetPos((int)(g_GlobeEularAngle.m_1_Horz/360.0*100));
    m_slider_roty_ctrl.SetPos((int)(g_GlobeEularAngle.m_2_Vert/360.0*100));
    m_slider_rotz_ctrl.SetPos((int)(g_GlobeEularAngle.m_3_Axis/360.0*100));
    return 1;  
} 
LRESULT CMyUniverseDlg::OnGlbUdpReadOnePage(WPARAM wParam, LPARAM lParam)  
{
    CString chapter = m_chapter_value;
    CString page = m_page_value;

    UpdateData(PUT_DATA);

    ReadPageStruct();
    m_page_value = page;

    UpdateData(PUT_DATA);

    ReadOnePage(false);
    return 1;
}