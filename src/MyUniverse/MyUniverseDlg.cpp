
// MyUniverseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyUniverse.h"
#include "MyUniverseDlg.h"
#include "afxdialogex.h"
#include "GlobeThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
    , edit_rotx(0)
    , edit_roty(0)
    , edit_rotz(0)
    , slider_rotx(0)
    , slider_roty(0)
    , slider_rotz(0)
    , story_path(_T(""))
    , chapter_value(_T(""))
    , page_value(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyUniverseDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ROTX, edit_rotx);
    DDV_MinMaxInt(pDX, edit_rotx, 0, 360);
    DDX_Text(pDX, IDC_EDIT_ROTY, edit_roty);
    DDV_MinMaxInt(pDX, edit_roty, 0, 360);
    DDX_Text(pDX, IDC_EDIT_ROTZ, edit_rotz);
    DDV_MinMaxInt(pDX, edit_rotz, 0, 360);
    DDX_Slider(pDX, IDC_SLIDER_ROTX, slider_rotx);
    DDX_Slider(pDX, IDC_SLIDER_ROTY, slider_roty);
    DDX_Slider(pDX, IDC_SLIDER_ROTZ, slider_rotz);
    DDX_Text(pDX, IDC_EDIT_SOTRY_PATH, story_path);
    DDV_MaxChars(pDX, story_path, 512);
    DDX_Control(pDX, IDC_COMBO_CHAPTER, chapter_select);
    DDX_CBString(pDX, IDC_COMBO_CHAPTER, chapter_value);
    DDV_MaxChars(pDX, chapter_value, 512);
    DDX_Control(pDX, IDC_COMBO_PAGE, page_select);
    DDX_CBString(pDX, IDC_COMBO_PAGE, page_value);
}

BEGIN_MESSAGE_MAP(CMyUniverseDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CMyUniverseDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CMyUniverseDlg::OnBnClickedCancel)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROTX, &CMyUniverseDlg::OnNMCustomdrawSliderRotx)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROTY, &CMyUniverseDlg::OnNMCustomdrawSliderRoty)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROTZ, &CMyUniverseDlg::OnNMCustomdrawSliderRotz)
    ON_EN_CHANGE(IDC_EDIT_ROTX, &CMyUniverseDlg::OnEnChangeEditRotx)
    ON_EN_CHANGE(IDC_EDIT_ROTY, &CMyUniverseDlg::OnEnChangeEditRoty)
    ON_EN_CHANGE(IDC_EDIT_ROTZ, &CMyUniverseDlg::OnEnChangeEditRotz)
    //ON_EN_CHANGE(IDC_EDIT1, &CMyUniverseDlg::OnEnChangeEdit1)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CMyUniverseDlg::OnBnClickedButtonBrowse)
    ON_CBN_SELCHANGE(IDC_COMBO_CHAPTER, &CMyUniverseDlg::OnCbnSelchangeComboChapter)
END_MESSAGE_MAP()


// CMyUniverseDlg ��Ϣ�������

BOOL CMyUniverseDlg::OnInitDialog()
{
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
    CreateThread(0, 0, GlobeThread, 0,0,0);//����OpenGL��ʾ�߳�

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
        CDialogEx::OnCancel();
    }
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotx(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    edit_rotx = (int)(slider_rotx/100.0*360);
    UpdateData(FALSE);
    *pResult = 0;
}


void CMyUniverseDlg::OnNMCustomdrawSliderRoty(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    edit_roty = (int)(slider_roty/100.0*360);
    UpdateData(FALSE);
    *pResult = 0;
}


void CMyUniverseDlg::OnNMCustomdrawSliderRotz(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    edit_rotz = (int)(slider_rotz/100.0*360);
    UpdateData(FALSE);
    *pResult = 0;
}


void CMyUniverseDlg::OnEnChangeEditRotx()
{
    UpdateData(TRUE);
    slider_rotx = (int)(edit_rotx/360.0*100);
    UpdateData(FALSE);
}


void CMyUniverseDlg::OnEnChangeEditRoty()
{
    UpdateData(TRUE);
    slider_roty = (int)(edit_roty/360.0*100);
    UpdateData(FALSE);
}


void CMyUniverseDlg::OnEnChangeEditRotz()
{
    UpdateData(TRUE);
    slider_rotz = (int)(edit_rotz/360.0*100);
    UpdateData(FALSE);
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
       story_path = szPath;
    }
    UpdateData(0);
    ReadChapterStruct();
}

void CMyUniverseDlg::ReadChapterStruct()
{
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

    chapter_select.ResetContent();

	hFind = FindFirstFile(story_path+"\\*", &FindFileData);
	
	if(hFind == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox ("Invalid file handle.\n");
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
                    chapter_select.AddString(FindFileData.cFileName);
                }
			}
		}while (FindNextFile(hFind, &FindFileData) != 0);

		DWORD dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			//printf ("FindNextFile error. Error is %u\n", dwError);
            AfxMessageBox("ERROR_NO_MORE_FILES");
		}
	}
    chapter_select.SetCurSel(0);
    ReadPageStruct();
}

void CMyUniverseDlg::ReadPageStruct()
{
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

    page_select.ResetContent();

    UpdateData(1);
    CString pagePath = story_path;
    int pathLength = pagePath.GetLength();
    char lastChar = *(pagePath.GetBuffer(0)+pathLength-1);
    if(lastChar != '\\')
    pagePath +="\\";
    pagePath += chapter_value;
    pagePath += "\\*";

	hFind = FindFirstFile(pagePath, &FindFileData);
	
	if(hFind == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox ("Invalid file handle.\n");
	}
	else
	{
		do
		{
            CString chapterName = FindFileData.cFileName;
            if(chapterName != "." && chapterName != "..")
            {
                page_select.AddString(FindFileData.cFileName);
			    if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
				    //printf ("����һ��Ŀ¼\n");
                    //page_select.AddString(FindFileData.cFileName);
			    }
            }
		}while (FindNextFile(hFind, &FindFileData) != 0);

		DWORD dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			//printf ("FindNextFile error. Error is %u\n", dwError);
            AfxMessageBox("ERROR_NO_MORE_FILES");
		}
	}
    page_select.SetCurSel(0);
}

void CMyUniverseDlg::OnCbnSelchangeComboChapter()
{
    ReadPageStruct();
}
